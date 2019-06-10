#!/usr/bin/env python3

import os, sys, gzip
import numpy as np
from pylab import *

if __name__ == '__main__':
	d = 'result/'
	gz = sorted([x for x in os.listdir(d) if 'gz' in x])
	log_time = []
	log_miss = []
	for id in [0,1,2,3,4,5]:
		log = open(d + '%d.log' % id).read().split('\n')[:-1]
		tag = []
		t = []
		miss = []
		for i in log[:]:
			a, b = int(i.split(' ')[0]), float(i.split(' ')[1])
			tag.append(a)
			t.append(b)
			file = [x for x in gz if i.split(' ')[0] in x][id]
			logs = gzip.open(d + file).read().decode().split('\n')
			result = float(logs[-6].split(' ')[-1])
			miss.append(result)
		log_time.append(t)
		log_miss.append(miss)
		print(id, np.mean(t), np.mean(miss))
		print(t)
		print(miss)
	log_time = np.array(log_time)
	base_time = log_time.max(axis=0)
	log_miss = np.array(log_miss)
	tag = np.array(tag)
	base_miss = log_miss.max(axis=0)
	figure()
	p = subplot(111)
	for i in range(6):
		p.plot(np.arange(tag.shape[0]), log_time[i], label=['LRU', 'Rand', 'FIFO', 'LFU', 'CLOCK', 'Ours'][i])
	p.legend()
	savefig('time.pdf')
	figure()
	p = subplot(111)
	for i in range(6):
		p.plot(np.arange(tag.shape[0]), log_miss[i], label=['LRU', 'Rand', 'FIFO', 'LFU', 'CLOCK', 'Ours'][i])
	p.legend()
	savefig('miss.pdf')
