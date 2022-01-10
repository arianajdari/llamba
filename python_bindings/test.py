from python_wrappers.multiplication import Multiplication


multiplication = Multiplication()


print(multiplication.execute_serial(data_size=400, iteration_number=5).toDict())
print(multiplication.execute_llamba(data_size=400, iteration_number=5, thread_number=4, scheduling_strategy=0, priority=18, affinity=False).toDict())
