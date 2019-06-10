#!/usr/bin/env python3
import web, json, time
from tomasulo import Tomasulo

urls = (
	"/", "Home",
	"/run", "Run",
	"/nel", "Nel",
	"/about", "About"
)

tmsl = Tomasulo()

class Home:
	def GET(self):
		raise web.seeother("/static/index.html")

class About:
	def GET(self):
		raise web.seeother("/static/about.html")

class Run:
	def POST(self):
		data = json.loads(web.data().decode())
		t = time.time()
		tmsl.run(int(data['step']))
		print('uses %lfs' % (time.time() - t))
		return tmsl.get_state()

class Nel:
	def POST(self):
		tmsl.set_nel(json.loads(web.data().decode())['nel'])
		return tmsl.get_state()

if __name__ == "__main__":
	web.application(urls, globals()).run()
