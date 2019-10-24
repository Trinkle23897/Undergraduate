#include <bits/stdc++.h>
int mcmf[100], dinic[100], rule[100];
int main(int argc, char const *argv[])
{
	freopen("mcmf.txt", "r", stdin);
	for (int i = 1; i <= 80; ++i)
		scanf("%*d%d", &mcmf[i]);
	freopen("dinic.txt", "r", stdin);
	for (int i = 1; i <= 80; ++i)
		scanf("%*d%d", &dinic[i]);
	freopen("rule.txt", "r", stdin);
	for (int i = 1; i <= 80; i++)
		scanf("%*d%d", &rule[i]);

	printf("ID\tmcmf\t\tdinic\t\trule\t\trule-mcmf\tdinic-rule\trule/mcmf %%\n");
	for (int i = 1; i <= 80; i++)
	{
		printf("%2d:\t%8d\t%8d\t%8d\t\t%3d\t\t%3d\t\t%.3lf\n",
		       i, mcmf[i] == 0 ? -1 : mcmf[i], dinic[i], rule[i],
		       mcmf[i] == 0 ? -1 : rule[i] - mcmf[i], dinic[i] - rule[i], mcmf[i] == 0 ? 0 : (rule[i] - mcmf[i]) * 100. / mcmf[i]);
	}
	return 0;
}
