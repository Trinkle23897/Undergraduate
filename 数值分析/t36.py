import numpy as np
from scipy.linalg import hilbert

def cholesky(A):
	# return np.linalg.cholesky(A)
	n = A.shape[0]
	L = np.zeros_like(A)
	for k in range(n):
		L[k, k] = (A[k, k] - ((L[k, :k] ** 2).sum() if k > 0 else 0)) ** .5
		for i in range(k + 1, n):
			L[i, k] = (A[i, k] - L[i, :k].dot(L[k, :k])) / L[k, k]
	return L

def solve(n, delta=0):
	print('n =', n, ', delta =', delta)
	H = hilbert(n)
	x = np.zeros(n) + 1 + delta
	b = H.dot(x)
	L = cholesky(H)
	# Hx = b, H = LL'
	# LL'x = b
	# x = L'^-1 L^-1 b
	xp = np.linalg.solve(L.T, np.linalg.solve(L, b))
	r = b - H.dot(xp)
	dx = xp - x
	print('||r||_inf:', abs(r).max())
	print('||dx||_inf:', abs(dx).max())

def gen36():
	print('t36 result'.center(30, '-'))
	solve(10)
	solve(10, 1e-7)
	solve(8)
	solve(12)
	print('end t36'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen36()
