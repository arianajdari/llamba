#ifndef ___BRESENHAM_HPP___
#define ___BRESENHAM_HPP___

#include <thread>
#include <vector>

#include <llamba/algorithms/bresenham.hpp>

#include "return_container.hpp"
#include "execution_time_calculation.hpp"

typedef std::milli TIME_MEASUREMENT;

class bresenham : protected execution_time_calculation {
    public:
        bresenham(int llamba_configuration) 
        {
            llamba::algorithm::init_thread_pool(llamba_configuration);
        }

        RESULT* execute_serial(int iteration_number, int step_size)
        {
            std::vector<unsigned long int> times;

            for(int i = 0; i < iteration_number; i++) {
                times.push_back(llamba::algorithm::bresenham_single_threaded(0,0,0, step_size, 30, 62));
                std::this_thread::sleep_for(std::chrono::operator""ms(500));
            }

            return calculate(times, step_size);
        } 

        RESULT* execute_llamba(int iteration_number, int step_size)
        {
            std::vector<unsigned long int> times;

            for(int i = 0; i < iteration_number; i++) {
                times.push_back(llamba::algorithm::bresenham_multithreading(0,0,0, step_size, 30, 62));
                std::this_thread::sleep_for(std::chrono::operator""ms(500));
            }

            return calculate(times, step_size);
        }
    private:
        bresenham()
        {

        }
};

#endif