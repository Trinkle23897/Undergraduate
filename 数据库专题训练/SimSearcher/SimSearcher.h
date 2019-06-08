#pragma once
#include <bits/stdc++.h>

#define ll long long
#define ld double
#define hash_t unsigned long long

#define C 37
#define L 270
#define N 200010
#define hash_seed 131
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define cmax(a, b) (a) < (b) ? ((a) = (b)) : 1
#define cmin(a, b) (a) > (b) ? ((a) = (b)) : 1

const int SUCCESS = 0;
const int FAILURE = 1;

struct TrieNode {
	int depth, time;
	TrieNode *ch[C], *fail;
	std::vector<int>* index;
	TrieNode(int dep) {
		depth = dep;
		time = -1;
		fail = NULL;
		index = NULL;
		memset(ch, 0, sizeof ch);
	}
};

struct Trie {
	TrieNode *root, *now;
	int from, to;
	Trie() {root = new TrieNode(0); root->fail = root; }
	inline void reset() { now = root; }
	inline int get_index(char c) {
		return c == ' ' ? 0 : c <= '9' ? c - '0' + 1 : c - 'a' + 11;
	}

	int insert(const char*s, int len, int id) {
		reset();
		for (int i = 0; i < len; ++i) {
			if (now->ch[get_index(s[i])] == NULL) {
				now->ch[get_index(s[i])] = new TrieNode(now->depth + 1);
			}
			now = now->ch[get_index(s[i])];
		}
		if (now->index == NULL) {
			now->index = new std::vector<int>();
			now->index->push_back(id);
			return 1;
		}
		else if (now->index->back() != id) {
			now->index->push_back(id);
			return 1;
		}
		return 0;
	}
	
	void build_AC() { // only for ED
		std::queue<TrieNode*> q;
		for (int i = 0; i < C; ++i)
			if (root->ch[i] != NULL) {
				root->ch[i]->fail = root;
				q.push(root->ch[i]);
			} else root->ch[i] = root;
		while (!q.empty()) {
			now = q.front(); q.pop();
			for (int i = 0; i < C; ++i) {
				if (now->ch[i] != NULL) {
					q.push(now->ch[i]);
					now->ch[i]->fail = now->fail->ch[i];
				}
				else
					now->ch[i] = now->fail->ch[i];
			}
		}
	}

	inline void step(char c) {
		now = now->ch[get_index(c)];
		if (now == NULL) now = root;
	}

	inline int find(const char*s, int len) {
		reset();
		for (int i = 0; i < len; ++i) {
			now = now->ch[get_index(s[i])];
			if (now == NULL)
				return 0;
		}
		return 1;
	}
};

class SimSearcher
{
public:
	int f_ED[L][L]; // DP
	int q, data_num, min_len, time_step;
	struct P4 {
		std::vector<int>* index; int num;
		void init(std::vector<int>* v, int n) { index = v, num = n; }
		bool operator < (const P4&a) const { return num < a.num; }
	} list_length[L];
	hash_t hash_token[L], *data_JC_id;
	char *data[N];
	int *data_len, *answer, *last_time, *candidate, *count; // avoid memset every time
	int *JC_num, *JC_index;
	struct P2 {
		int len, id;
		bool operator < (const P2&a) const { return len < a.len; }
	} *sort_len;
	// store ED & JC info
	Trie ED, JC;

	SimSearcher();
	~SimSearcher();
	inline int query_ED(char* s, const char* t, int len_s, int len_t, int thr);
	inline int query_JC(hash_t* s, hash_t* t, int len_s, int len_t, ld thr);
	inline int bin_search(std::vector<int>* data, int id);
	inline int lower_bound(int v);
	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, ld thr, std::vector<std::pair<unsigned, ld> > &result);
	int searchED(const char *query, unsigned thr, std::vector<std::pair<unsigned, unsigned> > &result);
};
