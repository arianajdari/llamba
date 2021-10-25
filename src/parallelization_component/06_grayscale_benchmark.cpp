#include <chrono>
#include <vector>
#include <iostream>
#include <llamba/base/grayscale_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include "Eigen/Dense"

#define DATA_TYPE int
#define DATA_SIZE 2000
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

void grayscale_eigen(Eigen::VectorXi & vec) 
{
    for(int i = 0; i < DATA_SIZE * DATA_SIZE; i++)
    {
        vec(i) = ((vec(i)&0x000000FF) + ((vec(i) >> 8 )&0x000000FF) + ((vec(i) >> 16 )&0x000000FF)) / 3;
    }
}

unsigned long int average(const std::vector<unsigned long int>& values)
{
    double average = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        average += values[i];
    }

    return average / ITERATION_NUMBER;
}

unsigned long int max(const std::vector<unsigned long int>& values)
{
    unsigned long int max = values[0];

    for(int i = 0; i < values.size(); i++)
    {
        if(values[i] > max) max = values[i];
    }

    return max;
}

unsigned long int min(const std::vector<unsigned long int>& values)
{
    unsigned long int min = values[0];

    for(int i = 0; i < values.size(); i++)
    {
        if(values[i] < min) min = values[i];
    }

    return min;
}

int main()
{   

    std::vector<unsigned long int> eigen_times;
    std::vector<unsigned long int> serial_times;
    std::vector<unsigned long int> llamba_times_standard;
    std::vector<unsigned long int> llamba_times_affinity;
    std::vector<unsigned long int> llamba_times_priority;
    std::vector<unsigned long int> llamba_times_affinity_priority;
    std::vector<unsigned long int> openmp_times;

    auto matrix_a  = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);
    auto matrix_b  = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);

    Eigen::VectorXi vec(DATA_SIZE * DATA_SIZE);

    for(int i = 0; i < DATA_SIZE * DATA_SIZE; i++)
    {
        vec(i) = i;
    }
    
    llamba::settings settings_serial            = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);
    llamba::settings settings_standard          = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 19_PRIORITY, AFFINITY::FALSE, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_affinity          = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 19_PRIORITY, AFFINITY::TRUE, std::vector<int>{0, 2, 4, 6}, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_priority          = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 80_PRIORITY, AFFINITY::FALSE, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_affinity_priority = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 80_PRIORITY, AFFINITY::TRUE, std::vector<int>{0, 2, 4, 6}, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_openmp            = llamba::settings(PARALLELIZATION_STRATEGY::OPENMP);

    std::cout << "Starting serial..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_serial);
        auto after = std::chrono::high_resolution_clock::now();
        serial_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Standard..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_standard);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_standard.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Affinity..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_affinity);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_affinity.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Priority..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_priority);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_priority.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Affinity and Priority..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_affinity_priority);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_affinity_priority.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting OpenMP..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_openmp);
        auto after = std::chrono::high_resolution_clock::now();
        openmp_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting Eigen..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto before = std::chrono::high_resolution_clock::now();
        grayscale_eigen(vec);
        auto after = std::chrono::high_resolution_clock::now();
        eigen_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    std::cout << "Done" << std::endl << std::endl;

    std::cout << "Grayscale Benchmark -- Matrix Size: (" << DATA_SIZE << "x" << DATA_SIZE << ")" << std::endl << std::endl;
    
    std::cout << "Serial                       times after " << ITERATION_NUMBER << " iterations: (min): " << min(serial_times) << "ms (max): " << max(serial_times)  << "ms (average): " << average(serial_times) << "ms" << std::endl;
    std::cout << "Llamba Standard              times after " << ITERATION_NUMBER << " iterations: (min): " << min(llamba_times_standard) << "ms (max): " << max(llamba_times_standard)  << "ms (average): " << average(llamba_times_standard) << "ms" << std::endl;
    std::cout << "Llamba Priority              times after " << ITERATION_NUMBER << " iterations: (min): " << min(llamba_times_priority) << "ms (max): " << max(llamba_times_priority)  << "ms (average): " << average(llamba_times_priority) << "ms" << std::endl;
    std::cout << "Llamba Affinity              times after " << ITERATION_NUMBER << " iterations: (min): " << min(llamba_times_affinity) << "ms (max): " << max(llamba_times_affinity)  << "ms (average): " << average(llamba_times_affinity) << "ms" << std::endl;
    std::cout << "Llamba Priority and Affinity times after " << ITERATION_NUMBER << " iterations: (min): " << min(llamba_times_affinity_priority) << "ms (max): " << max(llamba_times_affinity_priority)  << "ms (average): " << average(llamba_times_affinity_priority) << "ms" << std::endl;
    std::cout << "OpenMP                       times after " << ITERATION_NUMBER << " iterations: (min): " << min(openmp_times) << "ms (max): " << max(openmp_times)  << "ms (average): " << average(openmp_times) << "ms" << std::endl;
    std::cout << "Eigen                        times after " << ITERATION_NUMBER << " iterations: (min): " << min(eigen_times) << "ms (max): " << max(eigen_times)  << "ms (average): " << average(eigen_times) << "ms" << std::endl;

    std::cout << std::endl;

    std::cout << std::endl << "Press ctrl+c to exit the program...";

    return 0;
}