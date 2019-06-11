#include <bits/stdc++.h>
int main(int argc, char const *argv[])
{
	printf("----------t13 result----------\n");
	float s = 0, s0 = -1;
	int n = 0;
	for (; s0 != s;) {
		s0 = s;
		s += 1. / ++n;
	}
	printf("n = %d\n", n);
	double sd = 0;
	for (int i = 1; i <= n; ++i)
		sd += 1. / i;
	printf("float32: %.10lf\tfloat64: %.10lf\n", s, sd);
	double c0 = clock();
	sd = 0; n = 1e9;
	for (int i = 1; i <= n; ++i)
		sd += 1. / i;
	printf("inv 1e9: use %lfs\n", (clock() - c0) / CLOCKS_PER_SEC);
	printf("-----------end t13------------\n\n");
	return 0;
}
