import numpy as np
import scipy.special
from pylab import *

def fzerotx(func, a, b, eps=1e-10):
	fa, fb = func(a), func(b)
	c, fc = a, fa
	d = b - c
	e = d
	while fb != 0:
		if fa * fb > 0:
			a, fa, d = c, fc, b - c
			e = d
		if abs(fa) < abs(fb):
			c, fc = b, fb
			b, fb = a, fa
			a, fa = c, fc
		m = (a - b) / 2
		tol = 2 * eps * max(abs(b), 1)
		if abs(m) <= tol or fb == 0:
			break
		if abs(e) < tol or abs(fc) <= abs(fb):
			d, e = m, m
		else:
			s = fb / fc
			if a == c:
				p = 2 * m * s
				q = 1 - s
			else:
				q, r = fc / fa, fb / fa
				p = s * (2 * m * q * (q - r) - (b - c) * (r - 1))
				q = (q - 1) * (r - 1) * (s - 1)
			if p > 0:
				q = -q
			else:
				p = -p
			if 2 * p < 3 * m * q - abs(tol * q) and p < abs(e * q / 2):
				d, e = p / q, d
			else:
				d, e = m, m
		c, fc = b, fb
		if abs(d) > tol:
			b += d
		else:
			b -= sign(b - a) * tol
		fb = func(b)
	return b

def gen23():
	print('t23 result'.center(30, '-'))
	x = np.arange(0, 32, 0.1)
	y = scipy.special.j0(x)
	x_zero = []
	for i in range(x.shape[0] - 1):
		if y[i] * y[i+1] < 0:
			x_zero.append(fzerotx(scipy.special.j0, x[i], x[i+1]))
	x_zero = np.array(x_zero)
	for i in range(x_zero.shape[0]):
		print('#%d: x = %.10lf J0(x) = %.10lf' % (i + 1, x_zero[i], scipy.special.j0(x_zero[i])))
	figure()
	p = subplot(111)
	p.plot(x, y, '--')
	p.plot(x, np.zeros_like(x), '--')
	p.scatter(x_zero, np.zeros_like(x_zero))
	tight_layout()
	savefig('result/t23.png')
	print('end t23'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen23()
