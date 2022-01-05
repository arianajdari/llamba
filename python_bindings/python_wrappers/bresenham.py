from ctypes import *
from python_wrappers.result_struct import RESULT
  
# load the library
lib = cdll.LoadLibrary('./out/lib_bresenham.so')

# create a Multiplication class
class Bresenham(object):
  
    def __init__(self, configuration = 0):
        self.obj = lib.bresenham_create(configuration)
  
    def execute_serial(self, iteration_number, step_size):
        return RESULT.from_address(lib.bresenham_execute_serial(self.obj, iteration_number, step_size))

    def execute_llamba(self, iteration_number, step_size):
        return RESULT.from_address(lib.bresenham_execute_llamba(self.obj, iteration_number, step_size))