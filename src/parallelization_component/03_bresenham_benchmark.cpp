#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <llamba/algorithms/bresenham.hpp>

#define STEP_ONE    15000000
#define STEP_TWO    150000000
#define STEP_THREE  1500000000
#define STEP_FOUR   2000000000

#define LLAMBA_NO_CONFIGURATION 0
#define LLAMBA_PRIORITY 1
#define LLAMBA_AFFINITY 2
#define LLAMBA_PRIORITY_AFFINITY 3

int main()
{
    llamba::algorithm::init_thread_pool(LLAMBA_NO_CONFIGURATION);
    
    std::vector<double> times;

    std::cout << "Starting benchmarks... times in milliseconds" << std::endl << std::endl;

    
    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_multithreading(0,0,0, STEP_ONE, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_ONE, "Multi Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_multithreading(0,0,0, STEP_TWO, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_TWO, "Multi Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_multithreading(0,0,0, STEP_THREE, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_THREE, "Multi Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_multithreading(0,0,0, STEP_FOUR, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_FOUR, "Multi Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_single_threaded(0,0,0, STEP_ONE, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_ONE, "Single Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_single_threaded(0,0,0, STEP_TWO, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_TWO, "Single Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_single_threaded(0,0,0, STEP_THREE, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_THREE, "Single Threaded");
    times.clear();

    for(int i = 0; i < 10; i++) {
        times.push_back(llamba::algorithm::bresenham_single_threaded(0,0,0, STEP_FOUR, 30, 62));
        std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, STEP_FOUR, "Single Threaded");
    times.clear();

    std::cout << "Press ctrl+c to exit the program...\n";
    while(true) { }
    
    return 0;
}