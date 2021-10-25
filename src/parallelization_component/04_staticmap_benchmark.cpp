#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <llamba/algorithms/staticmap.hpp>

#define ITERATION_NUMBER 5

#define LLAMBA_NO_CONFIGURATION 0
#define LLAMBA_PRIORITY 1
#define LLAMBA_AFFINITY 2
#define LLAMBA_PRIORITY_AFFINITY 3

unsigned long int average(const std::vector<unsigned long int>& values, int iteration_number)
{
    double average = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        average += values[i];
    }

    return average / 5;
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
  llamba::algorithm::init_thread_pool(LLAMBA_NO_CONFIGURATION);

  std::cout << "Starting benchmarks... times in microseconds" << std::endl << std::endl;

  std::vector<unsigned long int> single_threaded_times;
  std::vector<unsigned long int> multi_threaded_times;

  for(int i = 0; i < ITERATION_NUMBER; i++)
  {
    single_threaded_times.push_back(llamba::algorithm::staticMapUpdate());
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  for(int i = 0; i < ITERATION_NUMBER; i++)
  {
    multi_threaded_times.push_back(llamba::algorithm::staticMapUpdateMultithread());
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  std::cout << "Serial times after " << ITERATION_NUMBER << " iterations: (min): " << min(single_threaded_times) << "micro (max): " << max(single_threaded_times)  << "micro (average): " << average(single_threaded_times, ITERATION_NUMBER) << "micro" << std::endl;
  std::cout << "Llamba times after " << ITERATION_NUMBER << " iterations: (min): " << min(multi_threaded_times) << "micro (max): " << max(multi_threaded_times)  << "micro (average): " << average(multi_threaded_times, ITERATION_NUMBER) << "mmicro" << std::endl;

  while(true) { }

  return 0;
}

