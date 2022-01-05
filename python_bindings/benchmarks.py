from python_wrappers.summation      import Summation
from python_wrappers.grayscale      import Grayscale
from python_wrappers.bresenham      import Bresenham
from python_wrappers.blue_channel   import BlueChannel
from python_wrappers.multiplication import Multiplication

# create a Multiplication class object
summation      = Summation()
grayscale      = Grayscale()
bresenham      = Bresenham()
blue_channel   = BlueChannel()
multiplication = Multiplication()

# object method calling

#result_llamba = multiplication.execute_llamba(800, 5)
#result_llamba_different = multiplication.execute_llamba(800, 5, thread_number=2, scheduling_strategy=0, affinity=True, priority=80)

result_llamba = bresenham.execute_llamba(10, 15000000)

print("min:" + str(result_llamba.min)+ " max:" + str(result_llamba.max) + " avg:" + str(result_llamba.avg))