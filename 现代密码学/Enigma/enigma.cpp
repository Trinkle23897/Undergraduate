#include <bits/stdc++.h>

#define N 26

struct engine {
	int k[3]; // unknown
	int sel[3]; // unknown
	char p[N]; // unknown
	char r[N]; // known
	char s[5][N]; // known
	engine() {
		// exchange
		for (int j = 0; j < N; ++j)
			p[j] = j;
		// for (int i = 0; i < 10; ++i) {
		// 	int x = rand() % N, y = rand() % N;
		// 	while (p[x] != x) x = rand() % N;
		// 	while (p[y] != y) y = rand() % N;
		// 	p[x] = y, p[y] = x;
		// }
		// rotor
		for (int j = 0; j < N; ++j) {
			//		   abcdefghijklmnopqrstuvwxyz
			s[0][j] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ"[j] - 'A';
			s[1][j] = "AJDKSIRUXBLHWTMCQGZNPYFVOE"[j] - 'A';
			s[2][j] = "BDFHJLCPRTXVZNYEIWGAKMUSQO"[j] - 'A';
			s[3][j] = "ESOVPZJAYQUIRHXLNFTGKDCMWB"[j] - 'A';
			s[4][j] = "VZBRGITYUPSDNHLXAWMJQOFECK"[j] - 'A';
		}
		// for (int i = 0; i < 5; ++i) {
		// 	for (int j = 0; j < N; ++j)
		// 		s[i][j] = j;
		// 	std::random_shuffle(s[i], s[i] + N);
		// 	std::random_shuffle(s[i], s[i] + N);
		// }
		// reflect
		for (int j = 0; j < N; ++j)
			//		abcdefghijklmnopqrstuvwxyz
			r[j] = "YRUHQSLDPXNGOKMIEBFZCWVJAT"[j] - 'A';
	}

	void set_sel(int s0, int s1, int s2) {
		sel[0] = s0;
		sel[1] = s1;
		sel[2] = s2;
	}

	void set_key(int key) {
		k[0] = key % N;
		k[1] = (key / N) % N;
		k[2] = (key / N / N) % N;
	}

	void clear_p() {
		for (int j = 0; j < N; ++j)
			p[j] = j;
	}

	int index(int i, int j) {
		if (j < 0 || j >= N)
			return -1;
		for (int l = 0; l < N; ++l)
			if (s[sel[i]][l % N] == (j + k[i]) % N)
				return (l + N - k[i]) % N;
		return -1;
	}

#define print_N(s) {\
	for (int j = 0; j < N; ++j)\
		printf("%c", j + 'A');\
	puts("");\
	for (int j = 0; j < N; ++j)\
		printf("%c", s + 'A');\
	puts("\n");\
}

	void print() {
		print_N(p[j]);
		for (int i = 0; i < 3; ++i)
			print_N(s[sel[i]][(j + k[i]) % N]);
		print_N(r[j]);
		for (int i = 2; i >= 0; --i)
			print_N(index(i, j));
		print_N(p[j]);
	}

	int inc_key(int i) {
		if (sel[i] == 0) return (k[i] = (k[i] + 1) % N) == 'R' - 'A';
		if (sel[i] == 1) return (k[i] = (k[i] + 1) % N) == 'F' - 'A';
		if (sel[i] == 2) return (k[i] = (k[i] + 1) % N) == 'W' - 'A';
		if (sel[i] == 3) return (k[i] = (k[i] + 1) % N) == 'K' - 'A';
		if (sel[i] == 4) return (k[i] = (k[i] + 1) % N) == 'A' - 'A';
	}

	int step(int x, int inc=1) {
		if (inc_key(0) && inc_key(1) && inc_key(2));
		x = p[x];
		// printf("r3: %c\n", x+'A');
		x = (s[sel[0]][(x + k[0]) % N] - k[0] + N) % N;
		// printf("r2: %c\n", x+'A');
		x = (s[sel[1]][(x + k[1]) % N] - k[1] + N) % N;
		// printf("r1: %c\n", x+'A');
		x = (s[sel[2]][(x + k[2]) % N] - k[2] + N) % N;
		// printf("r: %c\n", x+'A');
		x = r[x];
		// printf("after r: %c\n", x+'A');
		x = index(2, x);
		// printf("r1: %c\n", x+'A');
		x = index(1, x);
		// printf("r2: %c\n", x+'A');
		x = index(0, x);
		// printf("r3: %c\n", x+'A');
		x = p[x];
		// printf("p: %c\n", x+'A');
		return x;
	}

	void encrypto(char*origin, char*crypto, int length, int inc=1) {
		for (int i = 0; i < length; ++i)
			crypto[i] = step(origin[i], inc);
	}

} enigma;

#define MAX_LENGTH 100
#define MAX_DEPTH 10

void tonumber(char*origin, char*num, int length) {
	for (int i = 0; i < length; ++i)
		num[i] = ('a' <= origin[i] && origin[i] <= 'z') ? origin[i] - 'a' : (origin[i] - 'A');
}

int length, lut[1000][MAX_DEPTH], n_lut;
char origin[MAX_LENGTH], crypto[MAX_LENGTH], test[MAX_LENGTH], flag[N];

void print_tochar(char*s, int length) {
	for (int i = 0; i < length; ++i)
		printf("%c", s[i] + 'A');
	puts("");
}

int stack[MAX_DEPTH];
void dfs(int idx, char*origin, char*crypto) {
	if (idx > 1 && crypto[stack[idx - 1]] == origin[stack[0]]) {
		for (int i = 0; i < idx; ++i) {
			lut[n_lut][i] = stack[i];
			printf("%2d %c ", stack[i], origin[stack[i]] + 'A');
		}
		++n_lut;
		puts("");
	}
	if (idx == 5)
		return;
	for (int i = 0; i < length; ++i)
		if (origin[i] == crypto[stack[idx - 1]]) {
			stack[idx] = i;
			dfs(idx + 1, origin, crypto);
		}
}

void parse_circle(char*s) {
	memset(flag, 0, sizeof flag);
	for (int i = 0; i < N; ++i)
		if (!flag[i]) {
			int c = i, n = 1;
			flag[c] = 1;
			while(c = s[c], !flag[c])
				flag[c] = 1, ++n;
			printf("%d ", n);
		}
	printf("\n");
}

int swap_list[6][2], internal_key, swap_res_num;

void check() {
	enigma.set_key(internal_key);
	enigma.encrypto(origin, test, length);
	bool flag = true;
	for (int i = 0; i < length && flag; ++i)
		if (test[i] != crypto[i])
			flag = false;
	if (flag) {
		printf("find key: %d sel: %d %d %d plugboard: ", internal_key, enigma.sel[0], enigma.sel[1], enigma.sel[2]);
		for (int i = 0; i < N; ++i)
			if (enigma.p[i] < i)
				printf("%c <-> %c ", 'A'+i, 'A'+enigma.p[i]);
		puts("");
		print_tochar(test, length);
		print_tochar(crypto, length);
	}
}

void dfs_swap(int x) {
	check();
	if (x == swap_res_num) {
		return;
	}
	for (int i = (x == 0 ? 0 : swap_list[x-1][0]); i < N; ++i)
		if (enigma.p[i] == i) {
			swap_list[x][0] = i;
			for (int j = i + 1; j < N; ++j)
				if (enigma.p[j] == j) {
					swap_list[x][1] = j;
					std::swap(enigma.p[i], enigma.p[j]);
					dfs_swap(x + 1);
					std::swap(enigma.p[i], enigma.p[j]);
				}
		}
}

struct Record {
	int k, s0, s1, s2, num;
	bool operator < (const Record& r) const { return num > r.num; }
} record[26*26*26*5*4*3];

struct Swap {
	int a, b;
	bool operator < (const Swap& s) const { return a < s.a; }
	bool operator = (const Swap& s) const { return a == s.a && b == s.b; }
	void init(int _a, int _b) { if (_a < _b) a = _a, b = _b; else a = _b, b = _a; }
} swaps[1000];

void crack() {
	puts("---begin crack---");
	memset(lut, -1, sizeof lut);
	for (int i = 0; i < length; ++i) {
		stack[0] = i;
		dfs(1, origin, crypto);
	}
	printf("n_lut %d\n", n_lut);
	int test_num = 0;
	for (int k = 0; k < 26*26*26; ++k)
		for (int s0 = 0; s0 < 5; ++s0)
		for (int s1 = 0; s1 < 5; ++s1)
			if (s1 != s0)
		for (int s2 = 0; s2 < 5; ++s2)
			if (s2 != s1 && s2 != s0) {
				record[test_num].k = k;
				record[test_num].s0 = s0;
				record[test_num].s1 = s1;
				record[test_num].s2 = s2;
				record[test_num].num = 0;
				enigma.set_sel(s0, s1, s2);
				for (int n = 0; n < n_lut; ++n) {
					test[0] = origin[lut[n][0]];
					for (int i = 0; lut[n][i] != -1; ++i) {
						enigma.set_key(k);
						for (int _ = 0; _ < lut[n][i]; _++)
							enigma.step(0);
						test[0] = enigma.step(test[0]);
					}
					if (test[0] == origin[lut[n][0]])
						++record[test_num].num;
				}
				if (record[test_num].num >= 11) {
					int swaps_num = 0;
					internal_key = k;
					printf("k = %d s %d %d %d maybe true\n", k, s0, s1, s2);
					for (int n = 0, i; n < n_lut; ++n) {
						test[0] = origin[lut[n][0]];
						for (i = 0; lut[n][i] != -1; ++i) {
							printf("%c", test[i]+'A');
							enigma.set_key(k);
							for (int _ = 0; _ < lut[n][i]; _++)
								enigma.step(0);
							test[i + 1] = enigma.step(test[i]);
						}
						if (test[i] == origin[lut[n][0]]) {
							for (int j = 0; j < i; ++j)
								if (enigma.p[test[j]] == test[j] && test[j] != origin[lut[n][j]])
									swaps[swaps_num++].init(test[j], origin[lut[n][j]]);
						}
						printf("%c %d\n", test[i]+'A', test[i] == origin[lut[n][0]]);
					}
					std::sort(swaps, swaps + swaps_num);
					swap_res_num = 6;
					for (int i = 0; i < swaps_num; ++i)
						if (enigma.p[swaps[i].a] == swaps[i].a) {
							printf("change %c <-> %c\n", 'A'+swaps[i].a, 'A'+swaps[i].b);
							std::swap(enigma.p[swaps[i].a], enigma.p[swaps[i].b]);
							--swap_res_num;
						}
					printf("swap_res_num: %d\n", swap_res_num);
					if (swap_res_num <= 4)
						dfs_swap(0);
					for (int i = 0; i < N; ++i)
						enigma.p[i] = i;
				}
				++test_num;
			}
	// std::sort(record, record+test_num);
	// for (int i = 0; record[i].num >= 11; ++i)
	// 	printf("k %d s %d %d %d num %d\n", record[i].k, record[i].s0, record[i].s1, record[i].s2, record[i].num);
	// enigma.set_sel(2, 0, 1);
	// std::swap(enigma.p['A'-'A'], enigma.p['T'-'A']);
	// std::swap(enigma.p['D'-'A'], enigma.p['U'-'A']);
	// std::swap(enigma.p['I'-'A'], enigma.p['L'-'A']);
	// dfs_swap(0);
	puts("----end crack----");
}

int main(int argc, char *argv[])
{
	// basic check 
	// tonumber("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", origin, 30);
	// enigma.set_sel(4, 2, 3);
	// // enigma.set_key(0);
	// enigma.encrypto(origin, crypto, 30);
	// enigma.print();
	// print_tochar(crypto, 30);
	// return 0;
	// end for basic check
	length = 33;
	srand(0);
	tonumber("secretmessagesentheredatasecutiry", origin, length);
	tonumber("fsayuabiansukdnhbrqwwizqckivsdcxp", crypto, length);
	// int sel[5] = {0, 1, 2, 3, 4};
	// int key = rand() % (N * N * N);
	// std::random_shuffle(sel, sel+5);
	// enigma.set_sel(2, 0, 3);
	// // enigma.set_key(key);
	// enigma.k[0] = 'l' - 'a';
	// enigma.k[1] = 'r' - 'a';
	// enigma.k[2] = 'b' - 'a';
	// printf("%d\n", (enigma.k[2]*26+enigma.k[1])*26+enigma.k[0]);
	// char p[N];
	// for (int i = 0; i < N; ++i)
	// 	p[i] = enigma.p[i];
	// std::swap(enigma.p['b' - 'a'], enigma.p['q' - 'a']);
	// std::swap(enigma.p['c' - 'a'], enigma.p['r' - 'a']);
	// std::swap(enigma.p['d' - 'a'], enigma.p['i' - 'a']);
	// std::swap(enigma.p['e' - 'a'], enigma.p['j' - 'a']);
	// std::swap(enigma.p['k' - 'a'], enigma.p['w' - 'a']);
	// std::swap(enigma.p['m' - 'a'], enigma.p['t' - 'a']);
	// enigma.encrypto(origin, test, length);
	// enigma.print();
	// printf("origin: "); print_tochar(origin, length);
	// printf("test  : "); print_tochar(test, length);
	// printf("crypto: "); print_tochar(crypto, length);
	parse_circle(enigma.s[0]);
	parse_circle(enigma.s[1]);
	parse_circle(enigma.s[2]);
	parse_circle(enigma.s[3]);
	parse_circle(enigma.s[4]);
	// clear key
	// enigma.set_sel(0, 1, 2);
	// enigma.set_key(0);
	// enigma.clear_p();
	crack();
	// printf("True select: %d %d %d\n", sel[0], sel[1], sel[2]);
	// printf("True key: %d\n", key);
	// printf("True p: "); print_tochar(p, N);
	return 0;
}