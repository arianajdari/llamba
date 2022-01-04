#ifndef ___BLUE_CHANNEL_HPP___
#define ___BLUE_CHANNEL_HPP___

#include <chrono>
#include <vector>
#include <iostream>
#include <llamba/base/blue_channel_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include "Eigen/Dense"
#include "return_container.hpp"
#include "execution_time_calculation.hpp"

#define DATA_TYPE int

typedef std::milli TIME_MEASUREMENT;

class blue_channel : protected execution_time_calculation {
    public:
        RESULT* execute_serial(int data_size, int iteration_number) 
        {
            std::vector<unsigned long int> serial_times;

            auto matrix_a  = llamba::single_generator::generate_input<DATA_TYPE>(data_size);
            auto matrix_b  = llamba::single_generator::generate_input<DATA_TYPE>(data_size);

            llamba::settings settings_serial = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);

            for(int i = 0; i < iteration_number; i++)
            {
                auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(data_size);
                auto before = std::chrono::high_resolution_clock::now();
                llamba::base::blue_channel_parallel<DATA_TYPE> blue_channel_ = llamba::base::blue_channel_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_serial);
                auto after = std::chrono::high_resolution_clock::now();
                serial_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
            }

            return calculate(serial_times, data_size);
        }

        RESULT* execute_eigen(int data_size, int iteration_number) 
        {
            Eigen::VectorXi vec(data_size * data_size);

            for(int i = 0; i < data_size * data_size; i++)
            {
                vec(i) = i;
            }

            std::vector<unsigned long int> eigen_times;

            for(int i = 0; i < iteration_number; i++)
            {
                auto before = std::chrono::high_resolution_clock::now();
                blue_channel_eigen(vec, data_size);
                auto after = std::chrono::high_resolution_clock::now();
                eigen_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
            }

            return calculate(eigen_times, data_size);
        }

        RESULT* execute_openmp(int data_size, int iteration_number, int thread_number) 
        {
            std::vector<unsigned long int> openmp_times;

            auto matrix_a  = llamba::single_generator::generate_input<DATA_TYPE>(data_size);
            auto matrix_b  = llamba::single_generator::generate_input<DATA_TYPE>(data_size);

            llamba::settings settings_openmp = llamba::settings(thread_number, PARALLELIZATION_STRATEGY::OPENMP);

            for(int i = 0; i < iteration_number; i++)
            {
                auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(data_size);
                auto before = std::chrono::high_resolution_clock::now();
                llamba::base::blue_channel_parallel<DATA_TYPE> blue_channel_ = llamba::base::blue_channel_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_openmp);
                auto after = std::chrono::high_resolution_clock::now();
                openmp_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
            }

            return calculate(openmp_times, data_size);
        }

        RESULT* execute_llamba(int data_size, int iteration_number, int thread_number, int scheduling_strategy, int priority, bool affinity) 
        {
            std::vector<unsigned long int> llamba_times;

            auto matrix_a  = llamba::single_generator::generate_input<DATA_TYPE>(data_size);
            auto matrix_b  = llamba::single_generator::generate_input<DATA_TYPE>(data_size);

            llamba::settings settings_ = llamba::settings(thread_number, scheduling_strategy, priority, affinity, PARALLELIZATION_STRATEGY::PTHREADS);
                       
            for(int i = 0; i < iteration_number; i++)
            {
                auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(data_size);
                auto before = std::chrono::high_resolution_clock::now();
                llamba::base::blue_channel_parallel<DATA_TYPE> blue_channel_ = llamba::base::blue_channel_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_);
                auto after = std::chrono::high_resolution_clock::now();
                llamba_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
            }

            return calculate(llamba_times, data_size);
        }  
    private:
        void blue_channel_eigen(Eigen::VectorXi & vec, int data_size) 
        {
            for(int i = 0; i < data_size * data_size; i++)
            {   
                vec(i) = (vec(i) >> 16) & 0x000000FF;
            }
        }
};

#endif