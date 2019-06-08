#include "SimJoiner.h"

int main(int argc, char **argv) {
	SimJoiner joiner;

	std::vector<EDJoinResult> resultED;
	std::vector<JaccardJoinResult> resultJaccard;

	unsigned edThreshold = 3;
	double jaccardThreshold = 0.5;

	joiner.joinJaccard(argv[1], argv[2], jaccardThreshold, resultJaccard);
	joiner.joinED(argv[1], argv[2], edThreshold, resultED);

	for (const auto &i : resultED) {
		printf("%d %d %d\n", i.id1, i.id2, i.s);
	}

	for (const auto &i : resultJaccard) {
		printf("%d %d %lf\n", i.id1, i.id2, i.s);
	}

	return 0;
}
