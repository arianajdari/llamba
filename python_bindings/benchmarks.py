from python_wrappers.summation      import Summation
from python_wrappers.grayscale      import Grayscale
from python_wrappers.multiplication import Multiplication

# create a Multiplication class object
summation      = Summation()
grayscale      = Grayscale()
multiplication = Multiplication()

# object method calling

#result_llamba = multiplication.execute_llamba(800, 5)
#result_llamba_different = multiplication.execute_llamba(800, 5, thread_number=2, scheduling_strategy=0, affinity=True, priority=80)

result_llamba = grayscale.execute_llamba(4000, 125, thread_number=2)

print("min:" + str(result_llamba.min)+ " max:" + str(result_llamba.max) + " avg:" + str(result_llamba.avg))