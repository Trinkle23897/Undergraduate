from ctypes import *

libtaxi = cdll.LoadLibrary('../build/libtaxi.so')

init = libtaxi.init
init.argtypes = ()
init.restype = c_void_p

get_pos = libtaxi.get_pos
get_pos.argtypes = (c_double, c_double)
get_pos.restype = c_char_p

query = libtaxi.query
query.argtypes = (c_int, c_int)
query.restype = c_char_p
