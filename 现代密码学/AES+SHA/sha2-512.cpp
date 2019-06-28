#include <bits/stdc++.h>

#define size_t uint64_t
#define block_len 1024

size_t H[8], G[8], w[80];

const size_t K[80] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
	0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
	0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
	0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
	0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
	0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
	0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
};

char* padding(const char* s, int& l) {
	size_t orig = l = strlen(s);
	char* t = new char[l + block_len];
	memcpy(t, s, l);
	t[l++] = 0x80;
	while ((l << 3) % block_len != block_len - 128) ++l;
	l += 8;
	for (int i = 56; i >= 0; i -= 8)
		t[l++] = (char)((orig << 3) >> i);
	return t;
}

inline size_t ROTR(const size_t& t, const size_t& f) { return (t >> f) | (t << (64 - f)); }

char* sha(char* s, int len) {
	H[0] = 0x6a09e667f3bcc908, H[1] = 0xbb67ae8584caa73b, H[2] = 0x3c6ef372fe94f82b, H[3] = 0xa54ff53a5f1d36f1;
	H[4] = 0x510e527fade682d1, H[5] = 0x9b05688c2b3e6c1f, H[6] = 0x1f83d9abfb41bd6b, H[7] = 0x5be0cd19137e2179;
	char* ptr_w = (char*)(&w);
	for (int i = 0; i < len; i += 128) {
		for (int j = 0; j < 128; ++j)
			ptr_w[j] = s[i | (j ^ 7)];
		for (int j = 16; j < 80; ++j)
			w[j] = w[j - 16] + w[j - 7] + (ROTR(w[j - 15], 1) ^ ROTR(w[j - 15], 8) ^ (w[j - 15] >> 7)) + (ROTR(w[j - 2], 19) ^ ROTR(w[j - 2], 61) ^ (w[j - 2] >> 6));
		memcpy(G, H, sizeof H);
		for (int j = 0; j < 80; ++j) {
			size_t t2 = (ROTR(G[0], 28) ^ ROTR(G[0], 34) ^ ROTR(G[0], 39)) + ((G[0] & G[1]) ^ (G[1] & G[2]) ^ (G[2] & G[0]));
			size_t t1 = G[7] + (ROTR(G[4], 14) ^ ROTR(G[4], 18) ^ ROTR(G[4], 41)) + ((G[4] & G[5]) ^ ((~G[4]) & G[6])) + K[j] + w[j];
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
	char* ans = new char[128];
	for (int i = 0; i < 64; ++i) {
		uint8_t tmp = r[i ^ 7] & 0xff;
		ans[i << 1] = lut[tmp >> 4];
		ans[i << 1 | 1] = lut[tmp & 0xf];
	}
	return ans;
}

void test() {
	char* a = new char[1000000];
	for (int i = 0; i < 1000000; ++i) a[i] = 'a';
	assert(strcmp(hash(""), "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e") == 0);
	assert(strcmp(hash("a"), "1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75") == 0);
	assert(strcmp(hash("abc"), "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f") == 0);
	assert(strcmp(hash("message digest"), "107dbf389d9e9f71a3a95f6c055b9251bc5268c2be16d6c13492ea45b0199f3309e16455ab1e96118e8a905d5597b72038ddb372a89826046de66687bb420e7c") == 0);
	assert(strcmp(hash("abcdefghijklmnopqrstuvwxyz"), "4dbff86cc2ca1bae1e16468a05cb9881c97f1753bce3619034898faa1aabe429955a1bf8ec483d7421fe3c1646613a59ed5441fb0f321389f77f48a879c7b1f1") == 0);
	assert(strcmp(hash("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), "1e07be23c26a86ea37ea810c8ec7809352515a970e9253c26f536cfc7a9996c45c8370583e0a78fa4a90041d71a4ceab7423f19c71b9d5a3e01249f0bebd5894") == 0);
	assert(strcmp(hash("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"), "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445") == 0);
	assert(strcmp(hash(a), "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b") == 0);
	assert(strcmp(hash("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"), "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909") == 0);
	assert(strcmp(hash("abcdbcdecdefdefgefghfghighijhijk"), "c50e7a500d4058bf530ec603b66b032a989a3e033a340090dc51086cfd8cb22209027932ea830f9b6bc09dafe882f90838c2c91018245904828c1232fc0942eb") == 0);
	puts(hash("1234567890"));
	assert(strcmp(hash("1234567890"), "72ec1ef1124a45b047e8b7c75a932195135bb61de24ec0d1914042246e0aec3a2354e093d76f3048b456764346900cb130d2a4fd5dd16abb5e30bcb850dee843") == 0);
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
