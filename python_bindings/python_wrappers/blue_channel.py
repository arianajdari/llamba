from ctypes import *
from python_wrappers.result_struct import RESULT
  
# load the library
lib = cdll.LoadLibrary('./out/lib_blue_channel.so')

# create a Multiplication class
class BlueChannel(object):
  
    def __init__(self):
        self.obj = lib.blue_channel_create()
  
    def execute_serial(self, data_size, iteration_number):
        return RESULT.from_address(lib.blue_channel_execute_serial(self.obj, data_size, iteration_number))
    
    def execute_eigen(self, data_size, iteration_number):
        return RESULT.from_address(lib.blue_channel_execute_eigen(self.obj, data_size, iteration_number))
    
    def execute_openmp(self, data_size, iteration_number, thread_number = 4):
        return RESULT.from_address(lib.blue_channel_execute_openmp(self.obj, data_size, iteration_number, thread_number))

    def execute_llamba(self, data_size, iteration_number, thread_number=4, scheduling_strategy=0, priority=18, affinity=False):
        return RESULT.from_address(lib.blue_channel_execute_llamba(self.obj, data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity))