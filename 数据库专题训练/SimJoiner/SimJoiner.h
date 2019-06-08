#ifndef __EXP2_SIMJOINER_H__
#define __EXP2_SIMJOINER_H__

#include <bits/stdc++.h>
#define USE_CACHE 1
template <typename IDType, typename SimType>
class JoinResult { public:
	IDType id1, id2;
	SimType s;
	JoinResult(IDType a, IDType b, SimType c): id1(a), id2(b), s(c) {}
	bool operator < (const JoinResult&a) const { return id1 < a.id1 || (id1 == a.id1 && id2 < a.id2); }
};

typedef JoinResult<unsigned, double> JaccardJoinResult;
typedef JoinResult<unsigned, unsigned> EDJoinResult;

const int SUCCESS = 0, FAILURE = 1;

#define E 4 // ED < 4
#define C 10
#define L 260
#define H 99989
#define N 200010
#define M (1 << 21) // total word num
#define hash_seed 131
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define cmax(a, b) (a) < (b) ? ((a) = (b)) : 1
#define cmin(a, b) (a) > (b) ? ((a) = (b)) : 1
#define get_index(c) ((c >= 'a') ? (c) - 'a' : (c) >= '0' ? (c) - '0' + 26 : 36)
#define hash_t unsigned
#define ull unsigned long long

template<class A, class B>
struct Pair {
	A h; B id;
	Pair(): h(0), id(NULL) {}
	Pair(A _h, B _id): h(_h), id(_id) {}
	inline void init(A _h, B _id) { h = _h, id = _id; }
	bool operator < (const Pair&a) const { return h < a.h || (h == a.h && id < a.id); }
	bool operator == (const Pair&a) const { return h == a.h; }
};
typedef Pair<hash_t, int> P2;

class SimJoiner {
public:
	double JC_LIMIT;
	char* data[N];
	int isinit, words, ED_LIMIT, f_ED[L][L], data_num, *data_len, *word_num, *hash_index_JC, *hash_index_ED, *time_step, now_time, *count, *candidate;
	int *part_pos[L], *part_len[L], max_length, min_length;
	ull state[N];
	hash_t *hash_ED, hash_power[L];
	int *hash_JC;
	int *origin_trie_index;
	std::vector<P2> *part_vec[L];
	Pair<unsigned, int> idf[L];
	// for JC
	int *min_size;
	std::vector<int> **index;
	Pair<int, int> *sort_JC;
	int* inv_JC_id;
	SimJoiner();
	~SimJoiner();
	int global_init(const char*f1, unsigned thr);
	int joinJaccard(const char *filename1, const char *filename2, double threshold, std::vector<JaccardJoinResult> &result);
	int joinED(const char *filename1, const char *filename2, unsigned threshold, std::vector<EDJoinResult> &result);
	inline hash_t get_hash(hash_t*, int, int);
	inline int query_ED(char* s, char* t, int len_s, int len_t, int thr);
	inline int query_JC(int* s, int* t, int len_s, int len_t, double thr);
};

#endif