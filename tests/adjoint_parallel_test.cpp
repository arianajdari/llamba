#include <llamba/base/adjoint_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include <vector>
#include <iostream>

#define DATA_TYPE int
#define DATA_SIZE 4

void getCofactor(std::vector<DATA_TYPE>& A, std::vector<DATA_TYPE>& temp, int p, int q, int n)
{
    int i = 0, j = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i * DATA_SIZE + j++] = A[row * DATA_SIZE + col];
  
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int determinant(std::vector<DATA_TYPE>& A, int n)
{
    int D = 0; 

    if (n == 1)
        return A[0];
  
    std::vector<DATA_TYPE> temp(DATA_SIZE * DATA_SIZE); 
  
    int sign = 1; 
  
    for (int f = 0; f < n; f++)
    {
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0 * DATA_SIZE + f] * determinant(temp, n - 1);
  
        sign = -sign;
    }
  
    return D;
}
  
void adjoint_serial(std::vector<DATA_TYPE> A,std::vector<DATA_TYPE> adj)
{
    if (DATA_SIZE == 1)
    {
        adj[0] = 1;
        return;
    }
  
    int sign = 1; 
    std::vector<DATA_TYPE> temp(DATA_SIZE * DATA_SIZE);
  
    for (int i=0; i<DATA_SIZE; i++)
    {
        for (int j=0; j<DATA_SIZE; j++)
        {
            getCofactor(A, temp, i, j, DATA_SIZE);

            sign = ((i+j)%2==0)? 1: -1;
  
            adj[j * DATA_SIZE + i] = (sign)*(determinant(temp, DATA_SIZE-1));
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

    adjoint_serial(matrix_a, result_serial);
  
    llamba::base::adjoint_parallel<DATA_TYPE> adjoint_parallel_ = 
    llamba::base::adjoint_parallel<DATA_TYPE>(matrix_a, matrix_b, result_parallel, llamba::settings::make_settings());

    if(compare_results(result_serial, result_parallel))
        std::cout << "Serial Adjoint and Parallel Adjoint are equal" << std::endl;
    else
        std::cout << "Serial Adjoint and Parallel Adjoint are not equal" << std::endl;
  

  return 0;
}



