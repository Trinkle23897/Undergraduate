import os, sys, time

def exec(prefix, id):
	t = time.time()
	os.system('./bin/CMPsim.usetrace.64 -threads 1 -t traces/%s.out.trace.gz -o result/%s_%d -cache UL3:1024:64:16 -LLCrepl %d > /dev/null' % (prefix, prefix, id, id))
	print(prefix.split('.')[0], time.time() - t)

if __name__ == '__main__':
	test = sorted([x.split('.out')[0] for x in os.listdir('traces') if 'trace.gz' in x])
	id = int(sys.argv[-1])
	for t in test:
		exec(t, id)