#include <llamba/base/complex_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include <vector>
#include <iostream>

#define DATA_TYPE double
#define DATA_SIZE 100

void complex_serial(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b, std::vector<DATA_TYPE>& result_serial)
{
  for(int i = 0; i < DATA_SIZE; i++)
  {
    for(int j = 0; j < DATA_SIZE; j++)
    {
        result_serial[i * DATA_SIZE + j] =  matrix_a[i * DATA_SIZE + j] * matrix_b[i * DATA_SIZE + j] + pow(matrix_a[i * DATA_SIZE + j], 2);
    }
  }
}

bool compare_results(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b)
{
  bool isEqual = true;

  for(int i = 0; i < matrix_a.size(); i++)
  {
    if(matrix_a[i] != matrix_b[i])
    {
      isEqual = false;
      break;
    }
  }

  return isEqual;
}

int main()
{

  auto matrix_a = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);
  auto matrix_b = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);

  auto result_serial   = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
  auto result_parallel = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);

  complex_serial(matrix_a, matrix_b, result_serial);

  llamba::base::complex_parallel<DATA_TYPE> complex_parallel_ = 
    llamba::base::complex_parallel<DATA_TYPE>(matrix_a, matrix_b, result_parallel, llamba::settings::make_settings());
  
  if(compare_results(result_serial, result_parallel))
    std::cout << "Serial Complex Operation and Parallel Complex Operation are equal" << std::endl;
  else
    std::cout << "Serial Complex Operation and Parallel Complex Operation are not equal" << std::endl;
  
  return 0;
}



