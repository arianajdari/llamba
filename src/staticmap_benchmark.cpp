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

#define THREAD_POOL 10
#define THREAD_NUMBER 4
#define WIDTH 30

#define SIZE_Y 100
#define SIZE_X 20

std::mutex mutexes[THREAD_NUMBER];

struct iterators 
{
  iterators(
    uint8_t *& costs_,
    int size_y,
    int size_x  
  ) :
    costs_(costs_),
    size_y(size_y),
    size_x(size_x) 
  { }

  uint8_t *& costs_;
  int size_y;
  int size_x;
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


void staticMapUpdate()
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
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count() << std::endl; 
}

void staticMapUpdateMultithread() 
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
    
    for (unsigned int i = size_y; i < size_x; ++i) {
      for (unsigned int j = 0; j < 20; ++j) {
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
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(costs_, 0, 33); 
  iterators *iterators_2 = new iterators(costs_, 33, 67);
  iterators *iterators_3 = new iterators(costs_, 67, 100); 

  std::pair<staticmap_struct, iterators*> * p1 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_1);
  std::pair<staticmap_struct, iterators*> * p2 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_2);
  std::pair<staticmap_struct, iterators*> * p3 = new std::pair<staticmap_struct, iterators*>(settings_,iterators_3);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(after-before).count() << std::endl;


}


int main() 
{
  pthread_t threads[10];


  struct sched_param  sched_param_;
  sched_param_.sched_priority = 85;

  for(int i = 0; i < 10; i++)
  {
      int* arg = new int;
      *arg = i;
      tasks[i] = NULL;
     pthread_create(&threads[i], NULL, pool_thread_worker, (void*)arg);
      pthread_setschedparam(*(threads + i),0, &sched_param_);
  }

  for(int i = 0; i < 10; i++)
  {
      pthread_detach(threads[i]);
  }  

  while(true) {
   // staticMapUpdate();
   staticMapUpdateMultithread();
    std::this_thread::sleep_for(std::chrono::operator""ms(500));
  }

  return 0;
}

