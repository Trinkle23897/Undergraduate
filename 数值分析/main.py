#!/usr/bin/env python3

import os
from t11 import gen11
from t22 import gen22
from t23 import gen23
from t36 import gen36
from t42 import gen42
from t51 import gen51
from t53 import gen53
from t54 import gen54

def make(c):
	os.system('g++ %s.cpp -oa -O2; ./a; rm a' % (c))

if __name__ == '__main__':
	gen11()
	make('t13')
	gen22()
	gen23()
	gen36()
	gen42()
	gen51()
	gen53()
	gen54()
