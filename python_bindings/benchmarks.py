import json

from llamba_python_wrappers.summation      import Summation
from llamba_python_wrappers.grayscale      import Grayscale
from llamba_python_wrappers.bresenham      import Bresenham
from llamba_python_wrappers.blue_channel   import BlueChannel
from llamba_python_wrappers.multiplication import Multiplication

# 0 - llamba_base, 1 - priority, 2 - affinity, 3 - priority/affinity
bresenham_configuration = 0

# number of times to run tests
iteration_number = 1

multiplication = Multiplication()
summation      = Summation()
grayscale      = Grayscale()
blue_channel   = BlueChannel()
bresenham      = Bresenham(configuration=bresenham_configuration)

def perform_operation(dict, key, operation, data_size, iteration_number):
    if key not in dict.keys():
        dict[key] = {}
    dict[key][str(data_size) + "x" + str(data_size)] = {}
    dict[key][str(data_size) + "x" + str(data_size)]["serial"] = operation.execute_serial(data_size, iteration_number).toDict()
    dict[key][str(data_size) + "x" + str(data_size)]["eigen"]  =  operation.execute_eigen(data_size, iteration_number).toDict()
    dict[key][str(data_size) + "x" + str(data_size)]["openmp"] = operation.execute_openmp(data_size, iteration_number).toDict()
    dict[key][str(data_size) + "x" + str(data_size)]["llamba_base"] = operation.execute_llamba(data_size, iteration_number).toDict()
    dict[key][str(data_size) + "x" + str(data_size)]["llamba_affinity"] = operation.execute_llamba(data_size, iteration_number, affinity=True).toDict()
    dict[key][str(data_size) + "x" + str(data_size)]["llamba_priority"] = operation.execute_llamba(data_size, iteration_number, scheduling_strategy=1, priority=90).toDict()
    dict[key][str(data_size) + "x" + str(data_size)]["llamba_affinity_priority"] = operation.execute_llamba(data_size, iteration_number, affinity=True, scheduling_strategy=1, priority=90).toDict()

def perform_operation_only_llamba(dict, key, operation, data_size, iteration_number):
    if key not in dict.keys():
        dict[key] = {}
    dict[key][str(data_size)] = {}
    dict[key][str(data_size)]["serial"] = operation.execute_serial(data_size, iteration_number).toDict()
    dict[key][str(data_size)]["llamba_base"] = operation.execute_llamba(data_size, iteration_number).toDict()

if __name__ == '__main__':
    result = {}

    print("Starting Multiplication Benchmarks")
    perform_operation(result, "multiplication", multiplication, 100, iteration_number)
    perform_operation(result, "multiplication", multiplication, 200, iteration_number)
    print("Ending Multiplication Benchmarks")

    print("Starting Summation Benchmarks")
    perform_operation(result, "summation", summation, 1000, iteration_number)
    perform_operation(result, "summation", summation, 2000, iteration_number)
    print("Ending Summation Benchmarks")

    print("Starting Grayscale Benchmarks")
    perform_operation(result, "grayscale", grayscale, 1000, iteration_number)
    perform_operation(result, "grayscale", grayscale, 2000, iteration_number)
    print("Ending Grayscale Benchmarks")

    print("Starting Blue Channel Benchmarks")
    perform_operation(result, "blue_channel", blue_channel, 1000, iteration_number)
    perform_operation(result, "blue_channel", blue_channel, 2000, iteration_number)
    print("Ending Blue Channel Benchmarks")

    print("Starting Bresenham Benchmarks")
    perform_operation_only_llamba(result, "bresenham", bresenham, 1000000, iteration_number)
    perform_operation_only_llamba(result, "bresenham", bresenham, 2000000, iteration_number)
    print("Ending Bresenham Benchmarks")

    # Uncomment if you want to print the results
    #print(json.dumps(result))

    # Uncomment if you want to save the results in file
    #file = open("results.json", "w")
    #file.write(json.dumps(result))
    #file.close()