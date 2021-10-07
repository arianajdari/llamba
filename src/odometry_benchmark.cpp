#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <llamba/algorithms/odometry.hpp>

#define ITERATIONS_100000   100000
#define ITERATIONS_1000000  1000000
#define ITERATIONS_2000000  2000000
#define ITERATIONS_3000000  3000000

int main() 
{

  llamba::algorithm::init_thread_pool();



  std::vector<unsigned long int> times;

  //////////////////////////////////////////////////////
   times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdate(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_100000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_100000, "Single Threaded");

  times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdateMultithreading(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_100000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_100000, "Multi Threaded");
  
  ////////////////////////////////////////////////////////////

  times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdate(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_1000000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_1000000, "Single Threaded");
  

  times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdateMultithreading(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_1000000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_1000000, "Multi Threaded");

  /////////////////////////////////////////////////////////////////////////////
  times.clear();

  for(int i = 0; i < 10; i++) {
      times.push_back(llamba::algorithm::odometryUpdate(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_2000000));
      std::this_thread::sleep_for(std::chrono::operator""ms(500));
    }

  llamba::algorithm::calculate(times, ITERATIONS_2000000, "Single Threaded");

  times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdateMultithreading(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_2000000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_2000000, "Multi Threaded");
  //////////////////////////////////////////////////////////////////////////////   
  times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdate(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_3000000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_3000000, "Single Threaded");

  times.clear();

  for(int i = 0; i < 10; i++) {
    times.push_back(llamba::algorithm::odometryUpdateMultithreading(llamba::algorithm::pf, llamba::algorithm::pose, llamba::algorithm::delta, ITERATIONS_3000000));
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  llamba::algorithm::calculate(times, ITERATIONS_3000000, "Multi Threaded");

  //////////////////////////////////////////////////////////////////////////////   
  
  std::cout << "Press ctrl+c to exit the programm..." << std::endl;

  while(true) { }

  return 0;
}