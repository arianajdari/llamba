# LLAMBA

LLAMBA is a c++ header only library leveraging multithreading in order to help robotics engineers develop robotics applications. 

## Running benchmarks

The easiest way to run the benchmarks and check the results is to run provided python script benchmarks.py. In order to run this script, do the following:

```bash
 $ cd python_bindings
```

Before executing the file, check the comments inside benchmarks.py and decide whether you would like to print the results or save them to a file. After this, do the following:

```bash
 $ python3 benchmarks.py
```

## Usage of LLAMBA

Since LLAMBA is a header only library, it is enough to include its headers in the project. After including them, create a .cpp file and paste the following code:

```c++
#include  <llamba/base/multiply_parallel.hpp>
#include  <llamba/generators/single_generator.hpp>

#define DATA_SIZE 200

int main()
{
	auto matrix_a = llamba::single_generator::generate_input<double>(DATA_SIZE);
	auto matrix_b = llamba::single_generator::generate_input<double>(DATA_SIZE);
	auto result   = llamba::single_generator::generate_input<double>(DATA_SIZE);

	llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_serial);
	
	return 0;
}
```

Now you can compile the file with g++ compiler.


If you would like to use Benchmark Component, than you can use the code below (multiplication example):

```c++
#include  <vector>
#include  <llamba/generators/generator.hpp>
#include  <llamba/base/multiply_parallel.hpp>
#include  <llamba/benchmark/benchmark.hpp>

#define DATA_TYPE double

int main()
{
	auto matrices_a = llamba::generator::generate_inputs<DATA_TYPE>({100, 200});
	auto matrices_b = llamba::generator::generate_inputs<DATA_TYPE>({100, 200});
	
	llamba::settings input_settings_one = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);

	llamba::settings input_settings_two = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);

	llamba::benchmark<DATA_TYPE> benchmark_multiplication(new llamba::base::multiply_parallel<DATA_TYPE>(), matrices_a, matrices_b, std::vector<llamba::settings>{input_settings_one, input_settings_two});

	benchmark_multiplication.execute_all<TIME_FORMAT::MILLI>();
	return  0;
}
```