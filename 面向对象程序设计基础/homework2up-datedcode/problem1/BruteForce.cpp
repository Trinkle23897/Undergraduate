#include "BruteForce.h"
/**
 * [BF::polynomial description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @return   \sum_{i=0}^{n-1} a_i*x^i
 * low efficiency
 */
double BF::polynomial(int argc, double *argv)
{
	int n = argc - 2;
	double x = argv[argc - 1];
	double ans = 0;
	for (int i = 1; i <= n; ++i)
	{
		double pw = 1;
		for (int j = 1; j < i; ++j)
			pw *= x;
		ans += argv[i] * pw;
	}
	return ans;
}
/**
 * [BF::posynomial description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @return   \sum_{i=0}^{n-1} \frac{a_i}{x^i}
 * low efficiency
 */
double BF::posynomial(int argc, double *argv)
{
	int n = argc - 2;
	double x = argv[argc - 1];
	if (x == 0)
		return 1 / x;
	double ans = 0;
	for (int i = 1; i <= n; ++i)
	{
		double pw = 1;
		for (int j = 1; j < i; ++j)
			pw /= x;
		ans += argv[i] * pw;
	}
	return ans;
}
