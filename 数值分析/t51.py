import numpy as np

def solve(A):
	u = np.random.random(A.shape[0])
	lmd, lmd_old = 1e10, 0
	cnt = 0
	while abs(lmd - lmd_old) >= 1e-5:
		lmd_old = lmd
		v = A.dot(u)
		lmd = abs(v).max()
		u = v / lmd
		cnt += 1
	x = u
	return x, lmd, cnt

def gen51():
	print('t51 result'.center(30, '-'))
	A = np.array([
		[5, -4, 1],
		[-4, 6, -4],
		[1, -4, 7]
		])
	B = np.array([
		[25, -41, 10, -6],
		[-41, 68, -17, 10],
		[10, -17, 5, -3],
		[-6, 10, -3, 2]
		])
	xa, la, ca = solve(A)
	print('x_A:', xa, '\nlambda_A:', la, ', cnt_A:', ca)
	xb, lb, cb = solve(B)
	print('x_B:', xb, '\nlambda_B:', lb, ', cnt_B:', cb)
	print('end t51'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen51()
