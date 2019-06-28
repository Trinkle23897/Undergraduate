import numpy as np
from pylab import *
import sys, subprocess

def test(method, length):
	p = subprocess.Popen(['./sha%d %d' % (method, length)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
	stdout, stderr = p.communicate()
	return float(stdout.decode().split(' ')[1].split('s')[0])

def draw(length, sha256, sha512):
	figure()
	p = subplot(111)
	p.plot(length, sha256, '-o', label='SHA-256')
	p.plot(length, sha512, '-o', label='SHA-512')
	p.set_xlabel('Length')
	p.set_ylabel('Time')
	p.loglog()
	p.legend()
	tight_layout()
	savefig('SHA-log.png')

if __name__ == '__main__':
	np.set_printoptions(suppress=True, precision=6)
	lengths = 2 ** np.arange(10, 27)[7:]
	draw(lengths,
		np.array([0.001323, 0.001968, 0.002937, 0.005302, 0.010526, 0.020992, 0.042266, 0.084342, 0.168278, 0.336387]),
		np.array([0.001035, 0.001475, 0.001956, 0.003596, 0.00708,  0.014154, 0.028636, 0.057217, 0.114268, 0.228281])
		)
	lengths = [1048576]
	print(lengths)
	cnt = int(sys.argv[-1])
	for method in [256, 512]:
		result = []
		for length in lengths:
			s = 0
			for i in range(cnt):
				s += test(method, length)
			s /= cnt
			result.append(s)
		print(method, np.array(result))

# [  131072   262144   524288  1048576  2097152  4194304  8388608 16777216
#  33554432 67108864]
# 256 [0.001323 0.001968 0.002937 0.005302 0.010526 0.020992 0.042266 0.084342
#  0.168278 0.336387]
# 512 [0.001035 0.001475 0.001956 0.003596 0.00708  0.014154 0.028636 0.057217
#  0.114268 0.228281]
