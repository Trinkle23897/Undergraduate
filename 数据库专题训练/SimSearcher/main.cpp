#include "SimSearcher.h"

using namespace std;

#define print_result(vec) {\
	for (auto i: vec)\
		printf("%d ", i.first);\
	puts("");\
}

int main(int argc, char **argv)
{
	SimSearcher searcher;
	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 2;
	double jaccardThreshold = 0.85;
	double t = clock();
	searcher.createIndex(argv[1], q);
	fprintf(stderr, "%lf\n", (clock() - t) / CLOCKS_PER_SEC);
	char buf[300] = {0};
	freopen(argv[2], "r", stdin);
	while (gets(buf)) {
		// scanf("%d\n", &edThreshold);
		// searcher.searchED(buf, edThreshold, resultED);
		// print_result(resultED);
		scanf("%lf\n", &jaccardThreshold);
		searcher.searchJaccard(buf, jaccardThreshold, resultJaccard);
		print_result(resultJaccard);
	}
	fclose(stdin);

	return 0;
}
