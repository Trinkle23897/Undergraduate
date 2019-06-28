#include <bits/stdc++.h>

#define size_t uint32_t
#define block_len 512

size_t H[8], G[8], w[64];

const size_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

char* padding(const char* s, int& l) {
	size_t orig = l = strlen(s);
	char* t = new char[l + block_len];
	memcpy(t, s, l);
	t[l++] = 0x80;
	while ((l << 3) % block_len != block_len - 64) ++l;
	l += 4;
	for (int i = 24; i >= 0; i -= 8)
		t[l++] = (char)((orig << 3) >> i);
	return t;
}

inline size_t ROTR(const size_t& t, const size_t& f) { return (t >> f) | (t << (32 - f)); }

char* sha(char* s, int len) {
	H[0] = 0x6a09e667, H[1] = 0xbb67ae85, H[2] = 0x3c6ef372, H[3] = 0xa54ff53a;
	H[4] = 0x510e527f, H[5] = 0x9b05688c, H[6] = 0x1f83d9ab, H[7] = 0x5be0cd19;
	char* ptr_w = (char*)(&w);
	for (int i = 0; i < len; i += 64) {
		for (int j = 0; j < 64; ++j)
			ptr_w[j] = s[i | (j ^ 3)];
		for (int j = 16; j < 64; ++j)
			w[j] = w[j - 16] + w[j - 7] + (ROTR(w[j - 15], 7) ^ ROTR(w[j - 15], 18) ^ (w[j - 15] >> 3)) + (ROTR(w[j - 2], 17) ^ ROTR(w[j - 2], 19) ^ (w[j - 2] >> 10));
		memcpy(G, H, sizeof H);
		for (int j = 0; j < 64; ++j) {
			size_t t1 = G[7] + (ROTR(G[4], 6) ^ ROTR(G[4], 11) ^ ROTR(G[4], 25)) + ((G[4] & G[5]) ^ ((~G[4]) & G[6])) + K[j] + w[j];
			size_t t2 = (ROTR(G[0], 2) ^ ROTR(G[0], 13) ^ ROTR(G[0], 22)) + ((G[0] & G[1]) ^ (G[1] & G[2]) ^ (G[2] & G[0]));
			G[7] = G[6], G[6] = G[5], G[5] = G[4], G[4] = G[3] + t1;
			G[3] = G[2], G[2] = G[1], G[1] = G[0], G[0] = t1 + t2;
		}
		H[0] += G[0], H[1] += G[1], H[2] += G[2], H[3] += G[3];
		H[4] += G[4], H[5] += G[5], H[6] += G[6], H[7] += G[7];
	}
    return (char*) H;
}

const char* lut = "0123456789abcdef";

char* hash(const char* s) {
	int len = 0;
	char* t = padding(s, len);
	char* r = sha(t, len);
	char* ans = new char[64];
	for (int i = 0; i < 32; ++i) {
		uint8_t tmp = r[i ^ 3] & 0xff;
		ans[i << 1] = lut[tmp >> 4];
		ans[i << 1 | 1] = lut[tmp & 0xf];
	}
	return ans;
}

void test() {
	char* a = new char[1000000];
	for (int i = 0; i < 1000000; ++i) a[i] = 'a';
	assert(strcmp(hash(""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") == 0);
	assert(strcmp(hash("a"), "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb") == 0);
	assert(strcmp(hash("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad") == 0);
	assert(strcmp(hash("message digest"), "f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650") == 0);
	assert(strcmp(hash("abcdefghijklmnopqrstuvwxyz"), "71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73") == 0);
	assert(strcmp(hash("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), "db4bfcbd4da0cd85a60c3c37d3fbd8805c77f15fc6b1fdfe614ee0a7c8fdb4c0") == 0);
	assert(strcmp(hash("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"), "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1") == 0);
	assert(strcmp(hash(a), "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0") == 0);
	assert(strcmp(hash("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"), "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1") == 0);
	assert(strcmp(hash("abcdbcdecdefdefgefghfghighijhijk"), "b09cbd263b043f000c5befcaa40bc2f55a4785e024e5deb749b56061eafb65e9") == 0);
	puts(hash("1234567890"));
	assert(strcmp(hash("1234567890"), "f371bc4a311f2b009eef952dd83ca80e2b60026c8e935592d0f9c308453c813e") == 0);
}

int main(int argc, char const *argv[])
{
	if (atoi(argv[1]) == 0) test();
	else {
		int len = atoi(argv[1]);
		char* s = new char[len];
		for (int i = 0; i < len; ++i) s[i] = rand() % 26 + 'a';
		double t0 = clock();
		hash(s);
		printf("Uses %lfs\n", (clock() - t0) / CLOCKS_PER_SEC);
	}
	return 0;
}