#include <llamba/base/multiply_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include <vector>
#include <iostream>

#define DATA_TYPE double
#define DATA_SIZE 100

void multiply_serial(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b, std::vector<DATA_TYPE>& result_serial)
{
  for(int i = 0; i < DATA_SIZE; i++)
  {
    for(int j = 0; j < DATA_SIZE; j++)
    {
      for(int k = 0; k < DATA_SIZE; k++)
      {
        result_serial[i * DATA_SIZE + j] +=  matrix_a[i * DATA_SIZE + k] * matrix_b[k * DATA_SIZE + j];
      }
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

  multiply_serial(matrix_a, matrix_b, result_serial);

  llamba::base::multiply_parallel<DATA_TYPE> multiply_parallel_ = 
    llamba::base::multiply_parallel<DATA_TYPE>(matrix_a, matrix_b, result_parallel, llamba::settings::make_settings());
  
  if(compare_results(result_serial, result_parallel))
    std::cout << "Serial Multiplication and Parallel Multiplication are equal" << std::endl;
  else
    std::cout << "Serial Multiplication and Parallel Multiplication are not equal" << std::endl;
  
  return 0;
}



