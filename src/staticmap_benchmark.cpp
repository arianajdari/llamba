#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <llamba/algorithms/staticmap.hpp>

int main() 
{
  llamba::algorithm::init_thread_pool();

  while(true) {
    llamba::algorithm::staticMapUpdate();
    //llamba::algorithm::staticMapUpdateMultithread();
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  return 0;
}

