import time as tm
import numpy as np
from pylab import *

def Jacobi(A, b, x, eps=1e-4, xs=None):
	x = x.copy()
	cnt = 0
	while True:
		cnt += 1
		x_old = x.copy()
		for i in range(b.shape[0]):
			x[i] += (b[i] - A[i].dot(x_old)) / A[i, i] 
		if abs(x_old - x).max() < eps:
			return x, cnt

def GS(A, b, x, eps=1e-4, xs=None):
	x = x.copy()
	cnt = 0
	while True:
		cnt += 1
		x_old = x.copy()
		for i in range(b.shape[0]):
			x[i] += (b[i] - A[i].dot(x)) / A[i, i]
		if abs(x_old - x).max() < eps:
			return x, cnt

def SOR(A, b, x, eps=1e-4, w=0.9, xs=None):
	x = x.copy()
	cnt = 0
	while True:
		cnt += 1
		x_old = x.copy()
		for i in range(b.shape[0]):
			x[i] += w * (b[i] - A[i].dot(x)) / A[i, i]
		if abs(x_old - x).max() < eps:
			return x, cnt

def solve(eps, a, n):
	print('eps =', eps, ', a =', a, ', n =', n)
	A = np.zeros((n, n))
	h = 1 / n
	for i in range(n):
		A[i, i] = -2 * eps - h
	for i in range(n - 1):
		A[i + 1, i] = eps
		A[i, i + 1] = eps + h
	# print(A)
	x = np.arange(0 + h, 1 + h, h)
	y_star = (1 - a) / (1 - np.exp(-1 / eps)) * (1 - np.exp(-x / eps)) + a * x
	# print(A.dot(y_star))
	b = np.zeros(n) + a * h * h
	b[-1] -= eps + h
	y_j, c_j = Jacobi(A, b, x, xs=y_star)
	y_gs, c_gs = GS(A, b, x, xs=y_star)
	y_sor, c_sor = SOR(A, b, x, xs=y_star)
	print('cnt: JC %d\tGS %d\tSOR %d' % (c_j, c_gs, c_sor))
	y_star = np.concatenate([[1e-6], y_star])
	y_j = np.concatenate([[1e-6], y_j])
	y_gs = np.concatenate([[1e-6], y_gs])
	y_sor = np.concatenate([[1e-6], y_sor])
	return y_star, y_j, y_gs, y_sor

def gen42():
	print('t42 result'.center(30, '-'))
	a = .5
	n = 100
	h = 1 / n
	x = np.arange(0, 1 + h, h)
	y = []
	ystar = []
	for eps in [1, .1, .01, .001]:
		ys, yj, ygs, ysor = solve(eps, a, n)
		ystar.append(ys)
		y.append(yj)
		name = 'result/t42_eps' + str(eps) + '.png'
		figure()
		p = subplot(111)
		# p.plot(x, ys, '-', label='Truth', color='red')
		p.plot(x, abs(yj - ys) / ys, '-', label='Jacobi', color='green')
		p.plot(x, abs(ygs - ys) / ys, '-', label='GS', color='blue')
		p.plot(x, abs(ysor - ys) / ys, '-', label='SOR', color='orange')
		p.set_xlabel('x')
		p.set_ylabel('y')
		p.legend()
		tight_layout()
		savefig(name)
	figure()
	p = subplot(111)
	p.plot(x, y[0], '-', label=r'$\epsilon = 1$', color='red')
	p.plot(x, y[1], '-', label=r'$\epsilon = 0.1$', color='green')
	p.plot(x, y[2], '-', label=r'$\epsilon = 0.01$', color='blue')
	p.plot(x, y[3], '-', label=r'$\epsilon = 0.001$', color='orange')
	p.plot(x, ystar[0], '--', color='red')
	p.plot(x, ystar[1], '--', color='green')
	p.plot(x, ystar[2], '--', color='blue')
	p.plot(x, ystar[3], '--', color='orange')
	p.set_xlabel('x')
	p.set_ylabel('y')
	p.legend()
	tight_layout()
	savefig('result/t42.png')
	print('end t42'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen42()
