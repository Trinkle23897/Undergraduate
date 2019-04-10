#include "JiushaoQin.h"
/**
 * [Qin::polynomial description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @return   \sum_{i=0}^{n-1} a_i*x^i
 * high efficiency
 */
double Qin::polynomial(int argc, double *argv)
{
	int n = argc - 2;
	double x = argv[argc - 1];
	double ans = 0;
	for (int i = n; i; --i)
		ans = argv[i] + ans * x;
	return ans;
}
/**
 * [Qin::posynomial description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @return   \sum_{i=0}^{n-1} \frac{a_i}{x^i}
 * high efficiency
 */
double Qin::posynomial(int argc, double *argv)
{
	int n = argc - 2;
	double x = argv[argc - 1];
	if (x == 0)
		return 1 / x;
	double ans = 0;
	for (int i = n; i; --i)
		ans = argv[i] + ans / x;
	return ans;
}
