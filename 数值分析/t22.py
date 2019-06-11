import scipy.optimize

def newton(f, fp, x0, delta=1e-4):
	cnt = 0
	x = x0
	print('x0 = %.10lf' % x0)
	while abs(f(x)) > delta or abs(x - x0) > delta:
		s = f(x) / fp(x)
		x0, x = x, x - s
		lbd = 1
		while abs(f(x)) >= abs(f(x0)):
			x = x0 - lbd * s
			lbd /= 2
		cnt += 1
		print('x%d = %.10lf, lambda = %.10lf' % (cnt, x, lbd))
	return x

def gen22():
	def func1(x):
		return x**3 - x - 1
	def func1p(x):
		return 3*x*x - 1
	def func2(x):
		return -x**3 + 5 * x
	def func2p(x):
		return -3*x*x + 5
	
	print('t22 result'.center(30, '-'))
	x = newton(func1, func1p, 0.6)
	print('final result: x = %.10lf, f(x) = %.10lf' % (x, func1(x)))
	print('scipy gives: %.10lf' % (scipy.optimize.fsolve(func1, 0.6)))
	x = newton(func2, func2p, 1.35)
	print('final result: x = %.10lf, f(x) = %.10lf' % (x, func2(x)))
	print('scipy gives: %.10lf' % (scipy.optimize.fsolve(func2, 1.35)))
	print('end t22'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen22()
