from ctypes import *
from python_wrappers.result_struct import RESULT
  
# load the library
lib = cdll.LoadLibrary('./out/lib_multiplication.so')

# create a Multiplication class
class Multiplication(object):
  
    def __init__(self):
        self.obj = lib.multiplication_create()
  
    def execute_serial(self, data_size, iteration_number):
        return RESULT.from_address(lib.multiplication_execute_serial(self.obj, data_size, iteration_number))
    
    def execute_eigen(self, data_size, iteration_number):
        return RESULT.from_address(lib.multiplication_execute_eigen(self.obj, data_size, iteration_number))
    
    def execute_openmp(self, data_size, iteration_number, thread_number = 4):
        return RESULT.from_address(lib.multiplication_execute_openmp(self.obj, data_size, iteration_number, thread_number))

    def execute_llamba(self, data_size, iteration_number, thread_number=4, scheduling_strategy=0, priority=18, affinity=False):
        return RESULT.from_address(lib.multiplication_execute_llamba(self.obj, data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity))