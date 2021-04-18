#include <llamba/base/determinant_parallel.hpp>
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

int determinant_serial(std::vector<DATA_TYPE> A, int n)
{
    int D = 0; 

    if (n == 1)
        return A[0];
  
    std::vector<DATA_TYPE> temp(DATA_SIZE * DATA_SIZE); 
  
    int sign = 1; 
  
    for (int f = 0; f < n; f++)
    {
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0 * DATA_SIZE + f] * determinant_serial(temp, n - 1);
  
        sign = -sign;
    }
  
    return D;
}
  

int main()
{

    auto matrix_a = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);
    auto matrix_b = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);

    auto result_parallel = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);

    int result_serial = determinant_serial(matrix_a, DATA_SIZE);
  
    llamba::base::determinant_parallel<DATA_TYPE> determinant_parallel_ = 
    llamba::base::determinant_parallel<DATA_TYPE>(matrix_a, matrix_b, result_parallel, llamba::settings::make_settings());

    if(result_serial == result_parallel[0])
        std::cout << "Serial Determinant and Parallel Determinant are equal" << std::endl;
    else
        std::cout << "Serial Determinant and Parallel Determinant are not equal" << std::endl;
  

  return 0;
}



