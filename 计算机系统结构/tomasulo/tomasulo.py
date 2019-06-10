import json
import numpy as np

class Tomasulo(object):
	def __init__(self):
		super(Tomasulo, self).__init__()
		self.nel = []
		self.cdb = []
		self.clock = 0
		self.current = 0
		self.f_val = [0] * 32
		self.f_stat = [0] * 32
		self.f_upd = [0] * 32
		self.lb = [{'Name': 'LB%d' % (_ + 1), 'Busy': 'No', 'Remain': '', 'FU': '', 'Op': '', 'Imm': '', 'Src': '', 'Time': '', 'Code': ''} for _ in range(3)]
		self.ars = [{'Name': 'Ars%d' % (_ + 1), 'Busy': 'No', 'Remain': '', 'FU': '', 'Op': '', 'Vj': '', 'Vk': '', 'Qj': '', 'Qk': '', 'Time': '', 'Code': ''} for _ in range(6)]
		self.mrs = [{'Name': 'Mrs%d' % (_ + 1), 'Busy': 'No', 'Remain': '', 'FU': '', 'Op': '', 'Vj': '', 'Vk': '', 'Qj': '', 'Qk': '', 'Time': '', 'Code': ''} for _ in range(3)]
		self.code_state = []
		self.time = {'LD': 3, 'JUMP': 1, 'ADD': 3, 'SUB': 3, 'MUL': 12, 'DIV': 40}
		# self.time = {'LD': 3, 'JUMP': 1, 'ADD': 3, 'SUB': 3, 'MUL': 4, 'DIV': 4}
		self.done = False
		self.fu = [
			{'Name': 'Load1', 'Code': -1}, 
			{'Name': 'Load2', 'Code': -1}, 
			{'Name': 'Add1', 'Code': -1}, 
			{'Name': 'Add2', 'Code': -1}, 
			{'Name': 'Add3', 'Code': -1}, 
			{'Name': 'Mult1', 'Code': -1}, 
			{'Name': 'Mult2', 'Code': -1}, 
		]
		self.state = {}

	def str2imm(self, s):
		assert s[:2] == '0x', '%s is not a hex number?' % s
		num = int(s, 16)
		if num >= 2 ** 31:
			num -= 2 ** 32
		assert -2 ** 31 <= num < 2 ** 31, '%s is out of range.' % s
		return num

	def str2reg(self, s):
		assert s[0] == 'F', '%s is not a register?' % s
		num = int(s[1:])
		assert 0 <= num < 32, 'Register %s does not exist.' % s
		return num

	def set_nel(self, data):
		if isinstance(data, str):
			data = data.split('\n')
		assert isinstance(data, list), 'NEL is not a list or a string?'
		self.reset()
		self.nel = []
		self.code_state = []
		for s in data:
			s = s.upper().replace('0X', '0x').replace(',', ' ').split()
			code = {}
			if len(s) == 0:
				continue
			if 'LD' == s[0]:
				code['op'] = s[0]
				code['reg1'] = self.str2reg(s[1])
				code['imm1'] = self.str2imm(s[2])
				code['orig'] = ' '.join(s)
			elif 'JUMP' == s[0]:
				code['op'] = s[0]
				code['imm1'] = self.str2imm(s[1])
				code['reg1'] = self.str2reg(s[2])
				code['imm2'] = self.str2imm(s[3])
				code['orig'] = ' '.join(s)
			elif s[0] in ['ADD', 'SUB', 'MUL', 'DIV']:
				code['op'] = s[0]
				code['reg1'] = self.str2reg(s[1])
				code['reg2'] = self.str2reg(s[2])
				code['reg3'] = self.str2reg(s[3])
				code['orig'] = ' '.join(s)
			if len(code) > 0:
				self.nel.append(code)
				self.code_state.append({'Issue': '', 'Exec Comp': '', 'Write Result': '', 'Rs': ''})

	def reset(self):
		self.current = 0
		self.clock = 0
		self.cdb = []
		self.done = False
		self.state = {}
		# code state
		for i in range(len(self.nel)):
			for j in self.code_state[i]:
				self.code_state[i][j] = ''
		# Res
		for i in self.lb + self.ars + self.mrs:
			for j in i:
				if j != 'Name':
					i[j] = 'No' if j == 'Busy' else ''
		# Reg
		for i in range(32):
			self.f_stat[i] = self.f_val[i] = self.f_upd[i] = 0
		# FU
		for i in range(len(self.fu)):
			self.fu[i]['Code'] = -1

	def get_res(self, op):
		if op == 'LD':
			rs = self.lb
		elif op in ['ADD', 'SUB', 'JUMP']:
			rs = self.ars
		elif op in ['MUL', 'DIV']:
			rs = self.mrs
		else:
			return None
		for r in rs:
			if r['Busy'] == 'No':
				return r
		return None

	def get_fu(self, op):
		fname = {'LD': 'Load', 'ADD': 'Add', 'SUB': 'Add', 'JUMP': 'Add', 'MUL': 'Mult', 'DIV': 'Mult'}[op]
		for i in range(len(self.fu)):
			if fname in self.fu[i]['Name'] and self.fu[i]['Code'] < 0:
				return self.fu[i]
		return None

	def issue(self):
		if self.current >= len(self.nel):
			return
		# check current inst
		op = self.nel[self.current]['op']
		if op == 'JUMP' and self.code_state[self.current]['Rs'] != '': # JUMP not finish
			return
		rs = self.get_res(op)
		if rs == None:
			return
		# reset code status
		self.code_state[self.current]['Rs'] = rs
		self.code_state[self.current]['Exec Comp'] = ''
		self.code_state[self.current]['Write Result'] = ''
		self.code_state[self.current]['Issue'] = self.clock
		# set res status
		rs['Busy'] = 'Yes'
		rs['Time'] = self.clock
		rs['Op'] = op
		rs['Remain'] = self.time[op]
		rs['Code'] = self.current
		if op == 'LD': # Imm
			rs['Imm'] = self.nel[self.current]['imm1']
		else: # Vj Vk Qj Qk
			if op == 'JUMP':
				f = self.nel[self.current]['reg1']
			else:
				f = self.nel[self.current]['reg2']
			if self.f_stat[f] != self.f_val[f]:
				rs['Vj'], rs['Qj'] = '', self.f_stat[f]
			else:
				rs['Vj'], rs['Qj'] = self.f_val[f], ''
			if op != 'JUMP':
				f = self.nel[self.current]['reg3']
				if self.f_stat[f] != self.f_val[f]:
					rs['Vk'], rs['Qk'] = '', self.f_stat[f]
				else:
					rs['Vk'], rs['Qk'] = self.f_val[f], ''
		if op != 'JUMP':
			f = self.nel[self.current]['reg1']
			self.f_stat[f] = rs['Name']
		# if can exec then exec
		fu = self.get_fu(op)
		if fu is not None and (op == 'LD' or rs['Qj'] == '' and rs['Qk'] == ''):
			self.assign(rs, fu)
		# calc next inst
		if op != 'JUMP':
			self.current += 1

	def assign(self, rs, fu):
		rs['FU'] = fu['Name']
		fu['Code'] = rs['Code']

	def finish(self, rs, op):
		if op == 0:
			for fu in self.fu:
				if fu['Name'] == rs['FU']:
					rs['FU'] = ''
					fu['Code'] = -1
					return
		if op == 1:
			code = rs['Code']
			# update reg
			f = self.nel[code]['reg1']
			if rs['Op'] != 'JUMP':
				if rs['Op'] == 'LD':
					imm = rs['Imm']
				elif rs['Op'] == 'ADD':
					imm = rs['Vj'] + rs['Vk']
				elif rs['Op'] == 'SUB':
					imm = rs['Vj'] - rs['Vk']
				elif rs['Op'] == 'MUL':
					imm = rs['Vj'] * rs['Vk']
				elif rs['Op'] == 'DIV':
					if rs['Vk'] == 0:
						imm = rs['Vj']
					else:
						if rs['Vj'] >= 0:
							imm = rs['Vj'] // rs['Vk']
						else:
							imm = -(-rs['Vj'] // rs['Vk'])
				if self.f_stat[f] == rs['Name']:
					self.f_stat[f] = imm
				if self.f_upd[f] < rs['Time']:
					self.f_val[f] = imm
					self.f_upd[f] = rs['Time']
				self.notify(f, rs['Name'], imm)
			else:
				if self.f_val[f] == self.nel[code]['imm1']:
					self.current += self.nel[code]['imm2']
				else:
					self.current += 1
			rs['Code'] = ''
			rs['Busy'] = 'No'
			rs['Remain'] = ''
			rs['Op'] = ''
			rs['Imm'] = ''
			if 'Vj' in rs.keys():
				rs['Vj'] = rs['Vk'] = ''
			self.code_state[code]['Rs'] = ''

	def update(self):
		for rs in self.lb + self.ars + self.mrs:
			if rs['FU'] != '' and (rs['Op'] == 'LD' or rs['Qj'] == '' and rs['Qk'] == ''):
				if rs['Remain'] > 0:
					rs['Remain'] -= 1
		for rs in self.lb + self.ars + self.mrs:
			if rs['FU'] != '' and (rs['Op'] == 'LD' or rs['Qj'] == '' and rs['Qk'] == ''):
				if rs['Remain'] == 0: # exec comp
					code = rs['Code']
					self.code_state[code]['Exec Comp'] = self.clock
					self.finish(rs, 0)
					self.cdb.append([code, rs])

	def clear_cdb(self):
		for info in self.cdb:
			code, rs = info
			self.code_state[code]['Write Result'] = self.clock
			self.finish(rs, 1)
		self.cdb = []

	def notify(self, f, res, imm):
		for i in self.ars + self.mrs:
			if i['Qj'] == res or i['Qk'] == res:
				if i['Qj'] == res:
					i['Vj'], i['Qj'] = imm, ''
				if i['Qk'] == res:
					i['Vk'], i['Qk'] = imm, ''
				# if i['Qj'] == '' and i['Qk'] == '':
				# 	i['Remain'] += 1
				if i['Vk'] == 0 and i['Op'] == 'DIV':
					i['Remain'] = 1 # DIV 0

	def lookup_fu(self):
		for rs in self.lb + self.mrs:
			if rs['Remain'] != '' and rs['Remain'] > 0 and rs['FU'] == '' and (rs['Op'] == 'LD' or rs['Qj'] == '' and rs['Qk'] == ''):
				fu = self.get_fu(rs['Op'])
				if fu is not None:
					self.assign(rs, fu)
		ars_wl = []
		ars_wl_time = []
		for rs in self.ars:
			if rs['Remain'] != '' and rs['Remain'] > 0 and rs['FU'] == '' and rs['Qj'] == '' and rs['Qk'] == '':
				ars_wl_time.append(rs['Time'])
				ars_wl.append(rs)
		index = np.argsort(ars_wl_time)
		ava = []
		for i in range(2, 5):
			if self.fu[i]['Code'] < 0:
				ava.append(self.fu[i])
		for i in range(min(len(index), len(ava))):
			self.assign(ars_wl[index[i]], ava[i])

	def check_done(self):
		if self.current < len(self.nel):
			return
		self.done = True
		for i in self.code_state:
			if i['Write Result'] == '':
				self.done = False
				return
		for i in self.lb + self.ars + self.mrs:
			if i['Busy'] == 'Yes':
				self.done = False
				return

	def step(self):
		if self.done:
			return
		self.clock += 1
		self.clear_cdb()
		self.update()
		self.lookup_fu()
		self.issue()
		self.check_done()
		self.get_state(False)

	def run(self, cnt=1):
		if cnt < 0:
			while not self.done:
				self.step()
		else:
			for _ in range(cnt):
				self.step()

	def get_state(self, op=True):
		# collect nel
		self.state['nel'] = []
		for i, n in enumerate(self.nel):
			self.state['nel'].append(n['orig'])
			# code status
			for j in self.code_state[i]:
				if j != 'Rs':
					index = 'Code-%d-%s' % (i, j)
					if index not in self.state.keys():
						self.state[index] = ''
					if self.state[index] == '':
						self.state[index] = self.code_state[i][j]
					elif self.state[index] != self.code_state[i][j] and self.code_state[i][j] != '':
						if isinstance(self.state[index], int):
							self.state[index] = '%d (%d)' % (self.code_state[i][j], self.state[index])
						else:
							last = str(self.state[index].split('(')[-1].split(')')[0])
							self.state[index] = '%d (%s)' % (self.code_state[i][j], last)
		if op == False:
			return
		# clock
		self.state['clock'] = self.clock
		self.state['done'] = self.done
		# Reg
		for i in range(32):
			self.state['F%s-Status' % i] = self.f_stat[i]
			self.state['F%s-Value' % i] = self.f_val[i]
		# Reservation
		for i in self.lb + self.ars + self.mrs:
			for j in i:
				if j != 'Name':
					self.state[i['Name'] + '-' + j] = i[j]
		state_str = json.dumps(self.state)
		return state_str
