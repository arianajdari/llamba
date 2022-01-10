import ctypes
from ctypes import *
from llamba_python_wrappers.result_struct import RESULT
  
lib = ctypes.CDLL('./out/lib_blue_channel.so')

class BlueChannel(object):
  
    def __init__(self):
        self.obj = lib.blue_channel_create()
  
    def execute_serial(self, data_size, iteration_number):
        lib.blue_channel_execute_serial.restype  = ctypes.c_int64
        lib.blue_channel_execute_serial.argtypes = [ctypes.c_int64, ctypes.c_int64]
        return RESULT.from_address(lib.blue_channel_execute_serial(self.obj, data_size, iteration_number))
    
    def execute_eigen(self, data_size, iteration_number):
        lib.blue_channel_execute_eigen.restype  = ctypes.c_int64
        lib.blue_channel_execute_eigen.argtypes = [ctypes.c_int64, ctypes.c_int64]
        return RESULT.from_address(lib.blue_channel_execute_eigen(self.obj, data_size, iteration_number))
    
    def execute_openmp(self, data_size, iteration_number, thread_number = 4):
        lib.blue_channel_execute_openmp.restype  = ctypes.c_int64
        lib.blue_channel_execute_openmp.argtypes = [ctypes.c_int64, ctypes.c_int64, ctypes.c_int64]
        return RESULT.from_address(lib.blue_channel_execute_openmp(self.obj, data_size, iteration_number, thread_number))

    def execute_llamba(self, data_size, iteration_number, thread_number=4, scheduling_strategy=0, priority=18, affinity=False):
        lib.blue_channel_execute_llamba.restype = ctypes.c_int64
        lib.blue_channel_execute_llamba.argtypes = [ctypes.c_int64, ctypes.c_int64, ctypes.c_int64, ctypes.c_int64, ctypes.c_int64, ctypes.c_bool]
        return RESULT.from_address(lib.blue_channel_execute_llamba(self.obj, data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity))