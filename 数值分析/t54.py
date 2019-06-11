import numpy as np
from t53 import QR

def solve(A):
	eps = 1e-6
	k = A.shape[0]
	cnt = 0
	while k > 1 and abs(A[k-1, k-2]) > eps and cnt < 5:
		s = A[k-1, k-1]
		A[:k, :k] -= np.identity(k) * s
		qk, rk = QR(A[:k, :k])
		A[:k, :k] = rk.dot(qk) + np.identity(k) * s
		cnt += 1
		if abs(A[k-1, k-2]) <= eps:
			k -= 1
	print('cnt =', cnt)
	return A.diagonal()

def gen54():
	print('t54 result'.center(30, '-'))
	# A = np.array([
	# 	[2.9766, 0.3945, 0.4198, 1.1159],
	# 	[0.3945, 2.7328, -0.3097, 0.1129],
	# 	[0.4198, -0.3097, 2.5675, 0.6079],
	# 	[1.1159, 0.1129, 0.6079, 1.7231],
	# 	])
	A = np.array([
		[.5, .5, .5, .5],
		[.5, .5, -.5, -.5],
		[.5, -.5, .5, -.5],
		[.5, -.5, -.5, .5]
		])
	print(solve(A))
	print('end t54'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen54()
