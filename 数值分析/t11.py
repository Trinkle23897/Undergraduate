import numpy as np
from pylab import *

def draw(name, h, f=np.sin, fp=np.cos, x=1, e=0.6e-7, t=np.float32, M=1):
	h = h.astype(t)
	figure()
	p = subplot(111)
	p.plot(h, M * h / 2, '--', label='Truncation Error')
	p.plot(h, 2 * e / h, '--', label='Rounding Error')
	p.plot(h, M * h / 2 + 2 * e / h, '--', label='Total Error')
	p.plot(h, abs((f(x + h).astype(t) - f(x).astype(t)) / h - fp(x).astype(t)), '-', label='Real Error')
	p.set_xlabel('h')
	p.set_ylabel('Error')
	p.loglog()
	p.legend()
	tight_layout()
	savefig(name)

def gen11(step=.1):
	print('t11 result'.center(30, '-'))
	draw('result/t11_float32.png', np.array([10**x for x in np.arange(-8, 1, step)]), e=2**-25, t=np.float32)
	draw('result/t11_float64.png', np.array([10**x for x in np.arange(-16, 1, step)]), e=2**-54, t=np.float64)
	print('end t11'.center(30, '-')+'\n')

if __name__ == '__main__':
	gen11()
