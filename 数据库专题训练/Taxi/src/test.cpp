#include <bits/stdc++.h>
#include "GPTree.h"

double c0;
#define T_BEGIN c0 = clock();
#define T_END(msg) 
int main(int argc, char const *argv[])
{
	c0 = clock();
	gpt_build();
	printf("build gptree: %lfs\n", (clock() - c0) / CLOCKS_PER_SEC);
	
	while (1) {
		int x, y;
		scanf("%d%d", &x, &y);
		if (x < 0 || y < 0) break;
		printf("gpt_search(%d, %d): %d\n", x, y, gpt_search(x, y));
		std::vector<int> path;
		gpt_find_path(x, y, path);
		for (int i: path)
			printf("%d ", i);
		puts("");
	}

	int n = gpt_getsize(), tot = 10000, ans = 0;
	c0 = clock();
	for (int _ = 0; _ < tot; ++_) {
		int x = rand() % n;
		int y = rand() % n;
		int dis = gpt_search(x, y);
		ans += dis;
	}
	printf("search %d times: avg %lfs\n", tot, (clock() - c0) / CLOCKS_PER_SEC / tot);
	return 0;
}