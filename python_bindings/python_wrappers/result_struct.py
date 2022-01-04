from ctypes import *

class RESULT(Structure):
    _fields_ = [('data_size', c_int)       ,
                ('iteration_number', c_int),
                ('min', c_double)          ,
                ('max', c_double)          ,
                ('avg', c_double)
                ]
  