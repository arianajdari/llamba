#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#define LLAMBA_NO_CONFIGURATION 0
#define LLAMBA_PRIORITY 1
#define LLAMBA_AFFINITY 2
#define LLAMBA_PRIORITY_AFFINITY 3

#include <llamba/algorithms/forward.hpp>

#include "Eigen/Dense"

int main()
{
    llamba::algorithm::init_thread_pool(LLAMBA_NO_CONFIGURATION);

    std::cout << "Starting benchmarks... times in microseconds" << std::endl << std::endl;

    std::vector<unsigned long int> times;

    a = Eigen::VectorXf(6);
    a << 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250;

    d = Eigen::VectorXf(6);
    d << 0.08920, 0, 0, 0.1100, 0.69475, 0.08250;

    alph = Eigen::VectorXf(6);
    alph << 3.14/2, 0, 0, 3.14/2, -3.14/2, 0;

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransSerial6());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 6, "Single Threaded"); 

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransParallel6());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 6, "Multi Threaded");

    a = Eigen::VectorXf(12);
    a << 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250, 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250;

    d = Eigen::VectorXf(12);
    d << 0.08920, 0, 0, 0.1100, 0.69475, 0.08250, 0.08920, 0, 0, 0.1100, 0.69475, 0.08250;

    alph = Eigen::VectorXf(12);
    alph << 3.14/2, 0, 0, 3.14/2, -3.14/2, 0, 3.14/2, 0, 0, 3.14/2, -3.14/2, 0;

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransSerial12());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 12, "Single Threaded");  

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransParallel12());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 12, "Multi Threaded");  




    a = Eigen::VectorXf(18);
    a << 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250, 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250, 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250;

    d = Eigen::VectorXf(18);
    d << 0.08920, 0, 0, 0.1100, 0.69475, 0.08250, 0.08920, 0, 0, 0.1100, 0.69475, 0.08250, 0.08920, 0, 0, 0.1100, 0.69475, 0.08250;

    alph = Eigen::VectorXf(18);
    alph << 3.14/2, 0, 0, 3.14/2, -3.14/2, 0, 3.14/2, 0, 0, 3.14/2, -3.14/2, 0, 3.14/2, 0, 0, 3.14/2, -3.14/2, 0;

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransSerial18());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 18, "Single Threaded");  

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransParallel18());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 18, "Multi Threaded");  



    a = Eigen::VectorXf(24);
    a << 0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250, 
         0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250, 
         0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250, 
         0, -0.425, -0.39225, 0.1100, 0.69475, 0.08250;

    d = Eigen::VectorXf(24);
    d << 0.08920, 0, 0, 0.1100, 0.69475, 0.08250, 
         0.08920, 0, 0, 0.1100, 0.69475, 0.08250, 
         0.08920, 0, 0, 0.1100, 0.69475, 0.08250, 
         0.08920, 0, 0, 0.1100, 0.69475, 0.08250;

    alph = Eigen::VectorXf(24);
    alph << 3.14/2, 0, 0, 3.14/2, -3.14/2, 0, 
            3.14/2, 0, 0, 3.14/2, -3.14/2, 0, 
            3.14/2, 0, 0, 3.14/2, -3.14/2, 0, 
            3.14/2, 0, 0, 3.14/2, -3.14/2, 0;

    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransSerial24());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 24, "Single Threaded");  


    times.clear();

    for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::HTransParallel24());
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

    llamba::algorithm::calculate(times, 24, "Multi Threaded");  


    std::cout << std::endl << "Press ctrl+c to exit the program..." << std::endl;

    while(true) {}

    

 
    return 0;
}