import json
from ctypes import *

class RESULT(Structure):
    _fields_ = [('data_size', c_int)       ,
                ('iteration_number', c_int),
                ('min', c_double)          ,
                ('max', c_double)          ,
                ('avg', c_double)
                ]
    def toJSONCompletely(self):
        return json.dumps({"data_size" : str(self.data_size) + "x" + str(self.data_size), "iteration_number" : self.iteration_number, "min" : self.min, "max" : self.max, "avg" : self.avg}, default=lambda o: o.__dict__, 
            sort_keys=True, indent=4)

    def toJSON(self):
        return json.dumps({"min" : self.min, "max" : self.max, "avg" : self.avg}, default=lambda o: o.__dict__, 
            sort_keys=True, indent=4)

    def toDictCompletely(self):
        return {"data_size" : str(self.data_size) + "x" + str(self.data_size), "iteration_number" : self.iteration_number, "min" : self.min, "max" : self.max, "avg" : self.avg}
    
    def toDict(self):
        return {"min" : self.min, "max" : self.max, "avg" : self.avg}
  