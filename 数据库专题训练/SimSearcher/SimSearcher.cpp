#include "SimSearcher.h"

SimSearcher::SimSearcher()
{
	time_step = 0;
	min_len = 1 << 30;
	f_ED[0][0] = 0;
	for (int i = 1; i < L; ++i)
		f_ED[i][0] = f_ED[0][i] = i;
	data_len = new int[N];
	JC_num = new int[N];
	JC_index = new int[N];
	last_time = new int[N];
	answer = new int[N];
	candidate = new int[N];
	count = new int[N];
	sort_len = new P2[N];
	char *buf = new char[N*L];
	for (int i = 0; i < N; ++i)
		data[i] = buf + L * i;
	data_JC_id = new hash_t[N*L];
}

SimSearcher::~SimSearcher()
{
}

inline int SimSearcher::query_ED(char* s, const char* t, int len_s, int len_t, int thr) {
	if (max(len_s - len_t, len_t - len_s) > thr) return 0;
	for (int i = 1; i <= len_s; ++i)
		for (int j = max(1, i - thr); j <= len_t && j <= i + thr; ++j) {
			f_ED[i][j] = f_ED[i-1][j-1] + 1;
			if (j < i + thr) cmin(f_ED[i][j], f_ED[i-1][j] + 1);
			if (j > i - thr) cmin(f_ED[i][j], f_ED[i][j-1] + 1);
			if (s[i-1] == t[j-1]) cmin(f_ED[i][j], f_ED[i-1][j-1]);
		}
	return f_ED[len_s][len_t] <= thr;
}

inline int SimSearcher::query_JC(hash_t* s, hash_t* t, int len_s, int len_t, ld thr) {
	if (min(len_s, len_t) < thr * max(len_s, len_t))
		return 0;
	int sim = 0;
	for (int i = 0, j = 0; i < len_s && j < len_t; s[i] == t[j] ? ++i, ++j : s[i] < t[j] ? ++i : ++j)
		if (s[i] == t[j]) ++sim;
	return sim >= thr * (len_s + len_t - sim);
}

inline int SimSearcher::bin_search(std::vector<int>* data, int id) {
	if (id < data->front() || data->back() < id)
		return 0;
	int l = 0, r = data->size();
	for (int mid; l < r; mid = (l + r) >> 1, (*data)[mid] < id ? l = mid + 1 : r = mid);
	return (*data)[l] == id;
}

inline int SimSearcher::lower_bound(int v) {
	int l = 0, r = data_num;
	for (int mid; l < r; mid = (l + r) >> 1, sort_len[mid].len < v ? l = mid + 1 : r = mid);
	return l;
}

int SimSearcher::createIndex(const char *filename, unsigned q_)
{
	q = 9;
	freopen(filename, "r", stdin);
	char* buf;
	int id = 0;
	hash_t* hash_token = data_JC_id;
	for (; gets(buf = data[id]); ++id) {
		int len = 0, num = 0;
		// for JC
		hash_t hash = 0;
		for (int last = 0; ; ++len)
			if (buf[len] == ' ' || buf[len] == 0) {
				if (JC.insert(buf + last, len - last, id))
					hash_token[num++] = hash;
				if (buf[len] == 0) break;
				last = len + 1;
				hash = 0;
			} else {
				hash = hash * hash_seed + buf[len] - ' ';
			}
		// for ED
		for (int i = q; i <= len; ++i)
			ED.insert(buf + i - q, q, id);
		data_len[id] = len;
		sort_len[id] = (P2) {len, id};
		JC_num[id] = num;
		std::sort(hash_token, hash_token + num);
		hash_token += num;
		JC_index[id + 1] = hash_token - data_JC_id;
		cmin(min_len, num);
	}
	fclose(stdin);
	data_num = id;
	std::sort(sort_len, sort_len + data_num);
	ED.build_AC();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned thr, std::vector<std::pair<unsigned, unsigned> > &result)
{
	result.clear();
	++time_step;
	int len = strlen(query), ans_num = 0;
	// calc Tmin
	int Tmin = len + 1 - (thr + 1) * q;
	if (Tmin <= 0) {
		// query sort by length
		int l = lower_bound(len - thr);
		int r = lower_bound(len + thr + 1);
		for (int i = l; i < r; ++i) {
			int id = sort_len[i].id;
			if (query_ED(data[id], query, data_len[id], len, thr))
				answer[ans_num++] = id;
		}
		std::sort(answer, answer + ans_num);
		for (int i = 0; i < ans_num; ++i)
			result.push_back(std::make_pair(answer[i], 0));
		return SUCCESS;
	}
	int token_len = 0;
	ED.reset();
	for (int i = 0; i < len; ++i) {
		ED.step(query[i]);
		if (ED.now->depth == q && ED.now->index != NULL && ED.now->index->size() > 0)
			list_length[token_len++].init(ED.now->index, ED.now->index->size());
	}
	if (token_len < Tmin)
		return SUCCESS;
	std::sort(list_length, list_length + token_len);
	int num_short_list = token_len - (Tmin - 1);
	int cand_num = 0;
	// select short list
	for (int i = 0; i < num_short_list; ++i) {
		for (int id: *(list_length[i].index))
			if (last_time[id] != time_step) {
				last_time[id] = time_step;
				candidate[cand_num++] = id;
				count[id] = 1;
			} else ++count[id];
	}
	// fprintf(stderr, "%d\n", cand_num);
	// for (int i = num_short_list; i < token_len && i < token_len - (Tmin - 3); ++i) {
	// 	// int size = list_length[i].index->size();
	// 	// if (cand_num * (token_len - num_short_list) >= size / 10 + (cand_num + size) * (token_len - num_short_list - 1))
	// 		// break;
	// 	++num_short_list;
	// 	for (int id: *(list_length[i].index))
	// 		if (last_time[id] != time_step) {
	// 			last_time[id] = time_step;
	// 			candidate[cand_num++] = id;
	// 			count[id] = 1;
	// 		} else ++count[id];
	// }
	// if (num_short_list - (token_len - (Tmin - 1)) >= 0)
	// fprintf(stderr, "%d\n", num_short_list - (token_len - (Tmin - 1)));
	// filter
	for (int i = 0; i < cand_num; ++i) {
		int id = candidate[i], cnt = count[id];
		if (max(data_len[id] - len, len - data_len[id]) > thr) continue;
		int Tmin_id = max(data_len[id], len) + 1 - (thr + 1) * q;
		if (cnt + token_len - num_short_list < Tmin_id) continue;
		// for (int j = num_short_list; j < token_len && cnt < Tmin_id && cnt + token_len - j >= Tmin_id; ++j)
		// 	cnt += bin_search(list_length[j].index, id);
		if (query_ED(data[id], query, data_len[id], len, thr))
			answer[ans_num++] = id;
	}
	std::sort(answer, answer + ans_num);
	for (int i = 0; i < ans_num; ++i)
		result.push_back(std::make_pair(answer[i], 0));
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, ld thr, std::vector<std::pair<unsigned, ld> > &result)
{
	result.clear();
	++time_step;
	int len = 0, num = 0, token_len = 0;
	hash_t hash = 0;
	JC.reset();
	for (int last = 0; ; ++len)
		if (query[len] == ' ' || query[len] == 0) {
			if (JC.find(query + last, len - last) && JC.now->index != NULL && JC.now->time != time_step) {
				JC.now->time = time_step;
				list_length[token_len++].init(JC.now->index, JC.now->index->size());
			}
			hash_token[num++] = hash;
			if (query[len] == 0) break;
			last = len + 1;
			hash = 0;
		} else {
			hash = hash * hash_seed + query[len] - ' ';
		}
	std::sort(hash_token, hash_token + num);
	num = std::unique(hash_token, hash_token + num) - hash_token;
	// calc Tmin
	int Tmin = max(ceil(token_len * thr), ceil((token_len + min_len) * thr / (1 + thr)));
	// printf("Tmin: %d token: %d\n", Tmin, num);
	int ans_num = 0;
	if (Tmin <= 0) {
		for (int i = 0; i < data_num; ++i)
			if (query_JC(data_JC_id + JC_index[i], hash_token, JC_num[i], num, thr))
				result.push_back(std::make_pair(i, 0));
		return SUCCESS;
	}
	if (token_len < Tmin)
		return SUCCESS;
	std::sort(list_length, list_length + token_len);
	int num_short_list = token_len - (Tmin - 1);
	int cand_num = 0;
	// select short list
	for (int i = 0; i < num_short_list; ++i) {
		for (int id: *(list_length[i].index))
			if (last_time[id] != time_step) {
				last_time[id] = time_step;
				candidate[cand_num++] = id;
				count[id] = 1;
			} else ++count[id];
	}
	// for (int i = num_short_list; i < token_len && i < token_len - (Tmin - 2); ++i) {
	// 	// int size = list_length[i].index->size();
	// 	// if (cand_num * (token_len - num_short_list) >= size / 10 + (cand_num + size) * (token_len - num_short_list - 1))
	// 		// break;
	// 	++num_short_list;
	// 	for (int id: *(list_length[i].index))
	// 		if (last_time[id] != time_step) {
	// 			last_time[id] = time_step;
	// 			candidate[cand_num++] = id;
	// 			count[id] = 1;
	// 		} else ++count[id];
	// }
	// if (num_short_list > token_len - (Tmin - 1))
		// fprintf(stderr, "%d\n", num_short_list - (token_len - Tmin + 1));
	// filter
	for (int i = 0; i < cand_num; ++i) {
		int id = candidate[i], cnt = count[id];
		if (min(JC_num[id], token_len) < thr * max(JC_num[id], token_len)) continue;
		// if (max(data_len[id] - len, len - data_len[id]) > thr) continue;
		int Tmin_id = ceil((JC_num[id] + token_len) * thr / (1 + thr));
		if (cnt + token_len - num_short_list < Tmin_id) continue;
		// for (int j = num_short_list; j < token_len && cnt < Tmin_id && cnt + token_len - j >= Tmin_id; ++j)
		// 	cnt += bin_search(list_length[j].index, id);
		if (query_JC(data_JC_id + JC_index[id], hash_token, JC_num[id], num, thr))
				answer[ans_num++] = id;
	}
	std::sort(answer, answer + ans_num);
	for (int i = 0; i < ans_num; ++i)
		result.push_back(std::make_pair(answer[i], 0));
	return SUCCESS;
}
