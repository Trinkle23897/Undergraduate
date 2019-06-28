#include <bits/stdc++.h>

uint8_t gf02(uint8_t b) { return b < 0x80 ? b << 1 : (b << 1) ^ 0x1b; }
uint8_t gf03(uint8_t b) { return gf02(b) ^ b; }
uint8_t gf05(uint8_t b) { return gf02(gf02(b)) ^ b; }
uint8_t gf09(uint8_t b) { return gf02(gf02(gf02(b))) ^ b; }
uint8_t gf0b(uint8_t b) { return gf02(gf02(gf02(b))) ^ gf02(b) ^ b; }
uint8_t gf0d(uint8_t b) { return gf02(gf02(gf02(b))) ^ gf02(gf02(b)) ^ b; }
uint8_t gf0e(uint8_t b) { return gf02(gf02(gf02(b))) ^ gf02(gf02(b)) ^ gf02(b); }

int main() {
	puts("gf02");
	for (int i = 0; i < 256; ++i, i % 16 == 0 ? puts("") : 1)
		printf("0x%02x, ", gf02(i));
	puts("gf03");
	for (int i = 0; i < 256; ++i, i % 16 == 0 ? puts("") : 1)
		printf("0x%02x, ", gf03(i));
	puts("gf09");
	for (int i = 0; i < 256; ++i, i % 16 == 0 ? puts("") : 1)
		printf("0x%02x, ", gf09(i));
	puts("gf0b");
	for (int i = 0; i < 256; ++i, i % 16 == 0 ? puts("") : 1)
		printf("0x%02x, ", gf0b(i));
	puts("gf0d");
	for (int i = 0; i < 256; ++i, i % 16 == 0 ? puts("") : 1)
		printf("0x%02x, ", gf0d(i));
	puts("gf0e");
	for (int i = 0; i < 256; ++i, i % 16 == 0 ? puts("") : 1)
		printf("0x%02x, ", gf0e(i));
}
