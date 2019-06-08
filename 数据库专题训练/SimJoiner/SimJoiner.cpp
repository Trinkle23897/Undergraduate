#include "SimJoiner.h"

SimJoiner::SimJoiner(): JC_LIMIT(0.75), isinit(0), words(0), ED_LIMIT(3) {
	f_ED[0][0] = 0;
	for (int i = 1; i < L; ++i)
		f_ED[i][0] = f_ED[0][i] = i;
	max_length = 0;
	min_length = 0xffff;
	hash_power[0] = 1;
	for (int i = 1; i < L; ++i)
		hash_power[i] = hash_power[i - 1] * hash_seed;
	hash_JC = new int[M];
	hash_ED = new hash_t[N * L];
	char *buf = new char[N * L];
	for (int i = 0; i < N; ++i)
		data[i] = buf + i * L;
	data_len = new int[N];
	word_num = new int[N];
	hash_index_JC = new int[N];
	hash_index_ED = new int[N];
	time_step = new int[N];
	count = new int[N];
	candidate = new int[N];
	min_size = new int[M];
	inv_JC_id = new int[N];
	sort_JC = new Pair<int, int>[N];
	memset(min_size, 63, M * sizeof(int));
	origin_trie_index = new int[H];
	memset(origin_trie_index, -1, sizeof(int) * H);
	int* tmp = new int[L * E];
	memset(tmp, -1, L * E * sizeof(int));
	for (int i = 0; i < L; ++i)
		part_pos[i] = tmp + i * E;
	tmp = new int[L * E];
	for (int i = 0; i < L; ++i)
		part_len[i] = tmp + i * E;
	std::vector<P2>* v = new std::vector<P2>[L * E];
	for (int i = 0; i < L; ++i)
		part_vec[i] = v + i * E;
}

SimJoiner::~SimJoiner() {
}

inline hash_t SimJoiner::get_hash(hash_t* h, int offset, int len) {
	return h[offset + len] - h[offset] * hash_power[len];
}

int SimJoiner::global_init(const char*fn, unsigned thr) {
	if (isinit) return 0;
	isinit = 1;
	ED_LIMIT = thr;
	freopen(fn, "r", stdin);
	int id = 0;
	for (char* buf; gets(buf = data[id]); ++id) {
		int len = 0, num_JC = hash_index_JC[id];
		int num_ED = hash_index_ED[id] + 1; // 0 padding first
		for (hash_t hash = 0; ; ++len) {
			if (buf[len] == ' ' || buf[len] == 0) {
				hash %= H;
				if (origin_trie_index[hash] == -1)
					hash_JC[num_JC++] = origin_trie_index[hash] = words++;
				else
					hash_JC[num_JC++] = origin_trie_index[hash];
				if (buf[len] == 0) break;
				hash = 0;
			} else {
				hash = hash * hash_seed + buf[len] - ' ';
			}
			state[id] ^= (1 << (get_index(buf[len])));
			hash_ED[num_ED] = hash_ED[num_ED - 1] * hash_seed + buf[len] - ' ';
			++num_ED;
		}
		std::sort(hash_JC + hash_index_JC[id], hash_JC + num_JC);
		word_num[id] = std::unique(hash_JC + hash_index_JC[id], hash_JC + num_JC) - hash_JC - hash_index_JC[id];
		data_len[id] = len;
		sort_JC[id].init(-word_num[id], id);
		hash_index_JC[id + 1] = hash_index_JC[id] + word_num[id];
		hash_index_ED[id + 1] = num_ED;
		cmin(min_length, len);
		cmax(max_length, len);
		// create partition-based index
		int base_len = len / (ED_LIMIT + 1), nr = len % (ED_LIMIT + 1), p = 0;
		for (int i = 0; i <= ED_LIMIT; ++i) {
			part_pos[len][i] = p;
			part_len[len][i] = base_len + (i + nr > ED_LIMIT);
			hash_t h = get_hash(hash_ED, hash_index_ED[id] + p, part_len[len][i]);
			part_vec[len][i].emplace_back(h, id);
			p += part_len[len][i];
		}
	}
	fclose(stdin);
	data_num = id;
	for (int i = min_length; i <= max_length; ++i)
		if (part_pos[i][0] != -1)
			for (int j = 0; j <= ED_LIMIT; ++j)
				std::sort(part_vec[i][j].begin(), part_vec[i][j].end());
	index = new std::vector<int>* [words];
	for (int i = 0; i < words; ++i)
		index[i] = new std::vector<int>();
	std::sort(sort_JC, sort_JC + data_num);
	return 0;
}

int SimJoiner::joinJaccard(const char *filename1, const char *filename2, double threshold, std::vector<JaccardJoinResult> &result) {
	assert(std::string(filename1) == std::string(filename2));
	JC_LIMIT = threshold;
	result.clear();
	global_init(filename1, 3);
	for (int _id = 0; _id < data_num; ++_id) {
		int id = sort_JC[_id].id;
		inv_JC_id[id] = _id;
		time_step[id] = ++now_time;
		int* now_hash = hash_JC + hash_index_JC[id];
		int len = word_num[id], now_min_size = 0xffff;
		for (int i = 0; i < len; ++i) {
			int uid = now_hash[i];
			cmin(now_min_size, min_size[uid]);
			idf[i].init(index[uid]->size(), uid);
		}
		int Tmin = ceil(max(double(len), (len + now_min_size) / (1 + JC_LIMIT)) * JC_LIMIT);
		if (Tmin <= len) {
			int cand_num = 0;
			int num_short_list = len - (Tmin - 1);
			std::nth_element(idf, idf + num_short_list, idf + len);
			for (int i = 0; i < len && i < num_short_list; ++i)
				for (int _tid: (*index[idf[i].id])) {
					int tid = sort_JC[_tid].id;
					if (word_num[tid] >= Tmin) {
						if (time_step[tid] != now_time) {
							time_step[tid] = now_time;
							count[tid] = 1;
							candidate[cand_num++] = tid;
						} else ++count[tid];
					}
				}
			for (int i = 0; i < cand_num; ++i) {
				int tid = candidate[i], cnt = count[tid];
				int Tmin_id = ceil((word_num[tid] + len) * JC_LIMIT / (1 + JC_LIMIT));
				if (cnt >= Tmin_id) {
					result.emplace_back(id, tid, 1);
					result.emplace_back(tid, id, 1);
					continue;
				}
				for (int j = num_short_list; j < len; ++j) {
					if (cnt + len - j < Tmin_id) break;
					cnt += std::binary_search(index[idf[j].id]->begin(), index[idf[j].id]->end(), inv_JC_id[tid]);
					if (cnt >= Tmin_id) {
						result.emplace_back(id, tid, 1);
						result.emplace_back(tid, id, 1);
						break;
					}
				}
			}
		}
		result.emplace_back(id, id, 1);
		for (int i = 0; i < len; ++i) {
			int uid = now_hash[i];
			cmin(min_size[uid], word_num[id]);
			index[uid]->push_back(_id);
		}
	}
	std::sort(result.begin(), result.end());
	return SUCCESS;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned threshold, std::vector<EDJoinResult> &result) {
	assert(std::string(filename1) == std::string(filename2));
	result.clear();
	global_init(filename1, threshold);
	for (int id = 0; id < data_num; ++id) {
		time_step[id] = ++now_time;
		hash_t* now_hash = hash_ED + hash_index_ED[id];
		int len = data_len[id];
		for (int target_len = max(min_length, len - ED_LIMIT); target_len <= len; ++target_len) {
			if (part_pos[target_len][0] == -1) continue;
			for (int i = 0; i <= ED_LIMIT; ++i) {
				int tpos = part_pos[target_len][i], tlen = part_len[target_len][i];
				std::vector<P2>& vec = part_vec[target_len][i];
				int l = max(0, tpos + max(-i, len - target_len - ED_LIMIT + i));
				int r = min(len - tlen, tpos + min(i, len - target_len + ED_LIMIT - i));
				for (int j = l; j <= r; ++j) {
					hash_t h = get_hash(now_hash, j, tlen);
					std::vector<P2>::iterator it = std::lower_bound(vec.begin(), vec.end(), (P2) {h, 0});
					if (it == vec.end()) continue;
					for (int k = it - vec.begin(); k < vec.size() && vec[k].h == h; ++k) {
						int tid = vec[k].id;
						if (time_step[tid] == now_time || (len == target_len && tid >= id)) continue;
						if (__builtin_popcount(state[id] ^ state[tid]) > ED_LIMIT * 2) { // impossible
							time_step[tid] = now_time;
							continue;
						}
						int ans_l = 0, ans_r = 0;
						if (i) ans_l = query_ED(data[id], data[tid], j, tpos, i);
						if (ans_l > i) continue;
						if (ED_LIMIT - i) ans_r = query_ED(data[id] + j + tlen, data[tid] + tpos + tlen, len - j - tlen, data_len[tid] - tpos - tlen, ED_LIMIT - i);
						if (ans_r > ED_LIMIT - i) continue;
						if (ans_l + ans_r <= ED_LIMIT) {
							time_step[tid] = now_time;
							result.emplace_back(id, tid, ans_l + ans_r);
							result.emplace_back(tid, id, ans_l + ans_r);
						}
					}
				}
			}
		}
		result.emplace_back(id, id, 0);
	}
	std::sort(result.begin(), result.end());
	return SUCCESS;
}

inline int SimJoiner::query_ED(char* s, char* t, int len_s, int len_t, int thr) {
	if (max(len_s - len_t, len_t - len_s) > thr) return 0;
	for (int i = 1; i <= len_s; ++i) {
		int flag = 0;
		for (int j = max(1, i - thr); j <= len_t && j <= i + thr; ++j) {
			f_ED[i][j] = f_ED[i-1][j-1] + 1;
			if (j < i + thr) cmin(f_ED[i][j], f_ED[i-1][j] + 1);
			if (j > i - thr) cmin(f_ED[i][j], f_ED[i][j-1] + 1);
			if (s[i-1] == t[j-1]) cmin(f_ED[i][j], f_ED[i-1][j-1]);
			flag |= (f_ED[i][j] <= thr);
		}
		if (!flag) return thr + 1;
	}
	return f_ED[len_s][len_t];
}

inline int SimJoiner::query_JC(int* s, int* t, int len_s, int len_t, double thr) {
	if (min(len_s, len_t) < thr * max(len_s, len_t))
		return 0;
	int sim = 0;
	for (int i = 0, j = 0; i < len_s && j < len_t; s[i] == t[j] ? ++i, ++j : s[i] < t[j] ? ++i : ++j)
		if (s[i] == t[j]) ++sim;
	return sim >= thr * (len_s + len_t - sim);
}
