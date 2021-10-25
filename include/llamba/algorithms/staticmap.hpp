#ifndef __STATICMAP_HPP__
#define __STATICMAP_HPP__

#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <utility>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <sched.h>

#define THREAD_POOL 4
#define THREAD_NUMBER 4
#define WIDTH 30

#define SIZE_Y 400
#define SIZE_X 400

namespace llamba
{

namespace algorithm
{
std::mutex mutexes[THREAD_NUMBER];

struct iterators 
{
  iterators(
    uint8_t *& costs_,
    int size_x,
    int size_y
  ) :
    costs_(costs_),
    size_x(size_x),
    size_y(size_y) 
  { }

  uint8_t *& costs_;
  int size_x;
  int size_y;
  
};

struct staticmap_struct
{
  void (*fn) (iterators*);
  int thread_number;
  bool is_finished;
  bool is_processing;
  
  staticmap_struct(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false), is_processing(false) {};
};

std::pair<staticmap_struct, iterators*>* tasks[THREAD_POOL];

void * pool_thread_worker(void * arg__)
{
  int* arg = (int*) arg__;
  while(true)
  {
    if(tasks[*arg] != NULL)
    {
      auto p = tasks[*arg];
      if(!p->first.is_finished)
      {
        p->first.fn(p->second);
        p->first.is_finished = true;   
      }
    }
  }

  return NULL;
}

uint8_t interpret_value(const int8_t value)
{
  if (true && value == 100) {
    return 255;
  } else if (!true && value == 100) {
    return 0;
  } else if (value >= 100) {
    return 254;
  } else if (true) {
    return 0;
  }

  double scale = static_cast<double>(value / 200);
  return static_cast<uint8_t>(scale * 200);
}

unsigned long int staticMapUpdate()
{
  uint8_t * costs_ = new uint8_t[SIZE_Y * SIZE_X];

  for(int i = 0; i < SIZE_Y * SIZE_X; i++)
  {
    costs_[i] = i;
  }

  unsigned int index = 0;

  auto before = std::chrono::high_resolution_clock::now();
  for (unsigned int i = 0; i < SIZE_Y; ++i) {
    for (unsigned int j = 0; j < SIZE_X; ++j) {
      unsigned char value;

      if (true && value == 100) {
        value = 255;
      } else if (!true && value == 100) {
        value =  0;
      } else if (value >= 100) {
        value = 254;
      } else if (true) {
        value = 0;
      }
      costs_[index] = value;
      ++index;
    }
  }
  auto after = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(after - before).count(); 
}

unsigned long int staticMapUpdateMultithread() 
{

  uint8_t * costs_ = new uint8_t[SIZE_Y * SIZE_X];

  for(int i = 0; i < SIZE_Y * SIZE_X; i++)
  {
    costs_[i] = i;
  }

  staticmap_struct  settings_ = staticmap_struct([](iterators * iterators_) -> void {
    
    unsigned int index = 0;

    uint8_t * costs_ = iterators_->costs_;

    unsigned int size_y = iterators_->size_y;
    unsigned int size_x = iterators_->size_x;
    
    for (unsigned int i = size_x; i < size_y; ++i) {
    for (unsigned int j = 0; j < SIZE_X; ++j) {
        unsigned char value = 255;

        if (true && value == 100) {
          value = 255;
        } else if (!true && value == 100) {
          value =  0;
        } else if (value >= 100) {
          value = 254;
        } else if (true) {
          value = 0;
        }
        costs_[index] = value;
        index++;
      }
    }
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(costs_, 0, (SIZE_X /4) * 1); 
  iterators *iterators_2 = new iterators(costs_, (SIZE_X /4) * 1, (SIZE_X /4) * 2);
  iterators *iterators_3 = new iterators(costs_, (SIZE_X /4) * 2, (SIZE_X /4) * 3);
  iterators *iterators_4 = new iterators(costs_, (SIZE_X /4) * 3, SIZE_X); 

  std::pair<staticmap_struct, iterators*> * p1 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_1);
  std::pair<staticmap_struct, iterators*> * p2 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_2);
  std::pair<staticmap_struct, iterators*> * p3 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_3);
  std::pair<staticmap_struct, iterators*> * p4 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_4);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  tasks[3] = p4;

  while(true) 
  {
      if(tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished && tasks[3]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::microseconds>(after-before).count();
}

void init_thread_pool(int configuration) 
{
    pthread_t threads[10];

    struct sched_param  sched_param_;
    sched_param_.sched_priority = 85;

    cpu_set_t cpuset;
    int cpu_counter = 0;

    for(int i = 0; i < 5; i++)
    {
        int* arg = new int;
        *arg = i;
        tasks[i] = NULL;
        pthread_create(&threads[i], NULL, pool_thread_worker, (void*)arg);
        if(configuration == 3) {
          pthread_setschedparam(*(threads + i),0, &sched_param_);
          CPU_SET(cpu_counter, &cpuset);
          pthread_setaffinity_np(*(threads + i), sizeof(cpuset), &cpuset);
          CPU_CLR(cpu_counter, &cpuset);
          cpu_counter += 2;
        } else if (configuration == 2) {
          CPU_SET(cpu_counter, &cpuset);
          pthread_setaffinity_np(*(threads + i), sizeof(cpuset), &cpuset);
          CPU_CLR(cpu_counter, &cpuset);
          cpu_counter += 2;
        } else if (configuration == 1) {
          pthread_setschedparam(*(threads + i),0, &sched_param_);
        } else if (configuration == 0) {
          continue;
        }      
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_detach(threads[i]);
    }  
}

}

}

#endif