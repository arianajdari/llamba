#include <chrono>
#include <vector>
#include <iostream>
#include <llamba/base/sum_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include "Eigen/Dense"

#define DATA_SIZE 4000
#define ITERATION_NUMBER 5

typedef std::milli TIME_MEASUREMENT;

std::vector<int> create_vector(int width)
{
    std::vector<int> a(width);

    for(int i = 0; i < width; i++)
    {
        a[i] = i + 1;
    }

    return a;
}

void serialSummation(std::vector<double>& result, const std::vector<double>& matrix_a, const std::vector<double>& matrix_b)
{
     for(int i = 0; i < DATA_SIZE; i++)
    {
        for(int j = 0; j < DATA_SIZE; j++)
        {
            result[i * DATA_SIZE + j] = matrix_a[i * DATA_SIZE + j] + matrix_b[i * DATA_SIZE + j];
        }
    }

}

void calculate(std::vector<unsigned long int>& times, const int & steps, const char * str)
{
    double min = times[0];
    double max = times[0];

    double average = 0.0;
    int length = times.size();

    for(int i = 0; i < length; i++) {
        if(min > times[i])
            min = times[i];
        if(max < times[i])
            max = times[i];
        
        average += times[i];
    } 

    std::cout << "Summation " << str << " - " << steps << " steps" << std::endl
              << "min: " << min << " max: " << max <<  " avg: " << average/length << std::endl << std::endl;

}

int main() 
{
    std::vector<unsigned long int> serial_times;
    std::vector<unsigned long int> eigen_times;
    std::vector<unsigned long int> openmp_times;
    std::vector<unsigned long int> llamba_times;

    Eigen::MatrixXd a = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);
    Eigen::MatrixXd b = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);

    auto matrix_a  = llamba::single_generator::generate_input<double>(DATA_SIZE);
    auto matrix_b  = llamba::single_generator::generate_input<double>(DATA_SIZE);
    llamba::settings settings_ = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_openmp = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::OPENMP);
    llamba::settings settings_serial = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::sum_parallel<double> multiply = llamba::base::sum_parallel<double>(matrix_a, matrix_b, result, settings_openmp);
        auto after = std::chrono::high_resolution_clock::now();
        openmp_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::sum_parallel<double> multiply = llamba::base::sum_parallel<double>(matrix_a, matrix_b, result, settings_serial);
        auto after = std::chrono::high_resolution_clock::now();
        serial_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::sum_parallel<double> multiply = llamba::base::sum_parallel<double>(matrix_a, matrix_b, result, settings_);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        serialSummation(result, matrix_a, matrix_b);
        auto after = std::chrono::high_resolution_clock::now();
        serial_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto before = std::chrono::high_resolution_clock::now();
        Eigen::MatrixXd c = a + b;
        auto after = std::chrono::high_resolution_clock::now();
        eigen_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    std::cout << "Summation Benchmark -- Matrix Size: (" << DATA_SIZE << "x" << DATA_SIZE << ")" << std::endl << std::endl;

    calculate(serial_times, ITERATION_NUMBER, "Serial");
    calculate(llamba_times, ITERATION_NUMBER, "LLAMBA");
    calculate(eigen_times, ITERATION_NUMBER, "Eigen");
    calculate(openmp_times, ITERATION_NUMBER, "OpenMP");
    calculate(llamba_times, ITERATION_NUMBER, "LLAMBA");

    std::cout << std::endl;
 
    return 0;
}