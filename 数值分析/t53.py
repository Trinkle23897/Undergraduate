import numpy as np

def householder(_A):
	A = _A.copy()
	n = A.shape[1]
	v = np.zeros_like(A)
	for k in range(n):
		sigma = np.sign(A[k, k]) * np.sqrt((A[k:, k] ** 2).sum())
		if (A[k:, k] ** 2).sum() < 1e-10:
			continue
		v[k, k] += sigma
		v[k:, k] += A[k:, k]
		beta = v[:, k].dot(v[:, k])
		for j in range(k, n):
			A[:, j] -= 2 * (v[:, k].dot(A[:, j]) / beta) * v[:, k]
	return A, v

def QR(A):
	r, _ = householder(A)
	# A=QR, Q=AR^{-1}
	q = A.dot(np.linalg.inv(r))
	# q, r = np.linalg.qr(A)
	return q, r

def check(A):
	for i in range(1, A.shape[0]):
		if abs(A[i, :i-1]).sum() > 1e-3:
			return False
	return True

def solve(A):
	cnt = 0
	while check(A) == False and cnt <= 1000:
		q, r = QR(A)
		A = r.dot(q)
		cnt += 1
	if cnt >= 1000:
		print('Cannot solve A!')
		return None
	return A.diagonal()

def gen53():
	print('t53 result'.center(30, '-'))
	# A = np.array([[4,0,0],[4,5,5],[2,5,5.]])
	# A = np.array([[1,0,0], [0,1,0], [0,0,1], [-1,1,0],[-1,0,1], [0,-1,1.]])
	# print(QR(A))
	# exit()
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
	print('end t53'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen53()
