#include <chrono>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>

#include <llamba/base/multiply_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include "Eigen/Dense"

#define DATA_SIZE 400
#define ITERATION_NUMBER 10

double average(const std::vector<double>& values)
{
    double average = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        average += values[i];
    }

    return average / ITERATION_NUMBER;
}

double standard_deviation(const std::vector<double>& values)
{
    double average_ = average(values);

    double standard_deviation = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        standard_deviation += pow(average_ - values[i], 2);
    }

    standard_deviation /= ITERATION_NUMBER;

    return sqrt(standard_deviation);
}

int main() 
{
    std::vector<double> eigen_times;
    std::vector<double> llamba_times;
    std::vector<double> openmp_times;

    Eigen::MatrixXd a = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);
    Eigen::MatrixXd b = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);

    auto matrix_a  = llamba::single_generator::generate_input<double>(DATA_SIZE);
    auto matrix_b  = llamba::single_generator::generate_input<double>(DATA_SIZE);
    llamba::settings settings_ = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_serial = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::OPENMP);

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        time_t start = clock();
        llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_serial);
        openmp_times.push_back((double)(clock() - start) / CLOCKS_PER_SEC * 1000);
    }
    
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        time_t start = clock();
        llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_);
        llamba_times.push_back((double)(clock() - start) / CLOCKS_PER_SEC * 1000);
    }

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        time_t start = clock();
        Eigen::MatrixXd c = a * b;
        eigen_times.push_back((double)(clock() - start) / CLOCKS_PER_SEC * 1000);
    }

    std::cout << "Multiplication Benchmark -- Matrix Size: (" << DATA_SIZE << "x" << DATA_SIZE << ")" << std::endl << std::endl;

    std::cout << "Eigen  times after " << ITERATION_NUMBER << " iterations (average): " << average(eigen_times) << "ms" << std::endl;
    std::cout << "Llamba times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_times) << "ms" << std::endl; 
    std::cout << "OpenMP times after " << ITERATION_NUMBER << " iterations (average): " << average(openmp_times) << "ms" << std::endl;

    std::cout << std::endl;

    std::cout << "Eigen  times after " << ITERATION_NUMBER << " iterations (standard deviation): " << standard_deviation(eigen_times) << "ms" << std::endl;
    std::cout << "Llamba times after " << ITERATION_NUMBER << " iterations (standard deviation): " << standard_deviation(llamba_times) << "ms" << std::endl; 
    std::cout << "OpenMP times after " << ITERATION_NUMBER << " iterations (standard deviation): " << standard_deviation(openmp_times) << "ms" << std::endl; 

    return 0;
}