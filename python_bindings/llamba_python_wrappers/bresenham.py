import ctypes
from ctypes import *
from llamba_python_wrappers.result_struct import RESULT
  
lib = ctypes.CDLL('./out/lib_bresenham.so')

class Bresenham(object):
  
    def __init__(self, configuration = 0):
        self.obj = lib.bresenham_create(configuration)
  
    def execute_serial(self, step_size, iteration_number):
        lib.bresenham_execute_serial.restype  = ctypes.c_int64
        lib.bresenham_execute_serial.argtypes = [ctypes.c_int64, ctypes.c_int64]
        return RESULT.from_address(lib.bresenham_execute_serial(self.obj, iteration_number, step_size))

    def execute_llamba(self, step_size, iteration_number):
        lib.bresenham_execute_llamba.restype  = ctypes.c_int64
        lib.bresenham_execute_llamba.argtypes = [ctypes.c_int64, ctypes.c_int64]
        return RESULT.from_address(lib.bresenham_execute_llamba(self.obj, iteration_number, step_size))