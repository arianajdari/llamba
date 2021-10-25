#ifndef __FORWARD_HPP__
#define __FORWARD_HPP__

#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <utility>
#include <vector>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

#include "Eigen/Dense"

#define THREAD_POOL 10
#define THREAD_NUMBER 4
#define WIDTH_6 6
#define WIDTH_12 12
#define WIDTH_18 18
#define WIDTH_24 24

Eigen::VectorXf a;
Eigen::VectorXf d;
Eigen::VectorXf alph;

Eigen::Matrix4f A_1;
Eigen::Matrix4f A_2;
Eigen::Matrix4f A_3;
Eigen::Matrix4f A_4;
Eigen::Matrix4f A_5;
Eigen::Matrix4f A_6;

Eigen::Matrix4f A_7;
Eigen::Matrix4f A_8;
Eigen::Matrix4f A_9;
Eigen::Matrix4f A_10;
Eigen::Matrix4f A_11;
Eigen::Matrix4f A_12;

Eigen::Matrix4f A_13;
Eigen::Matrix4f A_14;
Eigen::Matrix4f A_15;
Eigen::Matrix4f A_16;
Eigen::Matrix4f A_17;
Eigen::Matrix4f A_18;

Eigen::Matrix4f A_19;
Eigen::Matrix4f A_20;
Eigen::Matrix4f A_21;
Eigen::Matrix4f A_22;
Eigen::Matrix4f A_23;
Eigen::Matrix4f A_24;

typedef std::micro TIME_MEASUREMENT;

namespace llamba
{

namespace algorithm
{

std::mutex mutexes[THREAD_NUMBER];

struct iterators 
{
  iterators(int n, Eigen::MatrixXf  matrix, int c) : n(n), matrix(matrix), c(c) 
  { }

  int n; 
  Eigen::MatrixXf  matrix; 
  int c;
};

struct forward_struct
{
  void (*fn) (iterators*);
  int thread_number;
  bool is_finished;
  bool is_processing;
  
  forward_struct(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false), is_processing(false) {};
};

std::pair<forward_struct, iterators*>* tasks[THREAD_POOL];

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

Eigen::MatrixXf AH(int n, const Eigen::MatrixXf & matrix, int c)
{
    
    Eigen::Matrix4f T_a;
    Eigen::Matrix4f T_d;
    Eigen::Matrix4f Rzt;
    Eigen::Matrix4f Rxa;
    Eigen::Matrix4f A_1;

    T_a << 1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1;

    T_d << 1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1;
    
    T_a(0, 3) = a(n - 1);

    T_d(2, 3) = d(n - 1);

    Rzt << cos(matrix(n - 1, c)), -sin(matrix(n - 1, c)), 0, 0,
           sin(matrix(n - 1, c)),  cos(matrix(n - 1, c)), 0, 0,
                               0,                      0, 1, 0,
                               0,                      0, 0, 1;
    
    Rxa << 1,                0,                 0, 0,
           0, cos(alph(n - 1)), -sin(alph(n - 1)), 0,
           0, sin(alph(n - 1)),  cos(alph(n - 1)), 0,
           0,                0,                 0, 1;

    
    A_1 = T_d * Rzt * T_a * Rxa;

    return A_1;
}

Eigen::Matrix4f HTrans6(const Eigen::MatrixXf & matrix, int c)
{
    Eigen::Matrix4f A_1 = AH(1, matrix, c);
    Eigen::Matrix4f A_2 = AH(2, matrix, c);
    Eigen::Matrix4f A_3 = AH(3, matrix, c);
    Eigen::Matrix4f A_4 = AH(4, matrix, c);
    Eigen::Matrix4f A_5 = AH(5, matrix, c);
    Eigen::Matrix4f A_6 = AH(6, matrix, c);

    return A_1 * A_2 * A_3 * A_4 * A_5 * A_6;
}

Eigen::Matrix4f HTrans12(const Eigen::MatrixXf & matrix, int c)
{
    Eigen::Matrix4f A_1 = AH(1, matrix, c);
    Eigen::Matrix4f A_2 = AH(2, matrix, c);
    Eigen::Matrix4f A_3 = AH(3, matrix, c);
    Eigen::Matrix4f A_4 = AH(4, matrix, c);
    Eigen::Matrix4f A_5 = AH(5, matrix, c);
    Eigen::Matrix4f A_6 = AH(6, matrix, c);

    Eigen::Matrix4f A_7 = AH(7, matrix, c);
    Eigen::Matrix4f A_8 = AH(8, matrix, c);
    Eigen::Matrix4f A_9 = AH(9, matrix, c);
    Eigen::Matrix4f A_10 = AH(10, matrix, c);
    Eigen::Matrix4f A_11 = AH(11, matrix, c);
    Eigen::Matrix4f A_12 = AH(12, matrix, c);

    return  A_1 * A_2 * 
            A_3 * A_4 * 
            A_5 * A_6 *
            A_7 * A_8 *
            A_9 * A_10 *
            A_11 * A_12;
}

Eigen::Matrix4f HTrans18(const Eigen::MatrixXf & matrix, int c)
{
    Eigen::Matrix4f A_1 = AH(1, matrix, c);
    Eigen::Matrix4f A_2 = AH(2, matrix, c);
    Eigen::Matrix4f A_3 = AH(3, matrix, c);
    Eigen::Matrix4f A_4 = AH(4, matrix, c);
    Eigen::Matrix4f A_5 = AH(5, matrix, c);
    Eigen::Matrix4f A_6 = AH(6, matrix, c);

    Eigen::Matrix4f A_7 = AH(7, matrix, c);
    Eigen::Matrix4f A_8 = AH(8, matrix, c);
    Eigen::Matrix4f A_9 = AH(9, matrix, c);
    Eigen::Matrix4f A_10 = AH(10, matrix, c);
    Eigen::Matrix4f A_11 = AH(11, matrix, c);
    Eigen::Matrix4f A_12 = AH(12, matrix, c);

    Eigen::Matrix4f A_13 = AH(13, matrix, c);
    Eigen::Matrix4f A_14 = AH(14, matrix, c);
    Eigen::Matrix4f A_15 = AH(15, matrix, c);
    Eigen::Matrix4f A_16 = AH(16, matrix, c);
    Eigen::Matrix4f A_17 = AH(17, matrix, c);
    Eigen::Matrix4f A_18 = AH(18, matrix, c);

    return  A_1  * A_2  * 
            A_3  * A_4  * 
            A_5  * A_6  *
            A_7  * A_8  *
            A_9  * A_10 *
            A_11 * A_12 *
            A_13 * A_14 *
            A_15 * A_16 *
            A_17 * A_18;
}

Eigen::Matrix4f HTrans24(const Eigen::MatrixXf & matrix, int c)
{
    Eigen::Matrix4f A_1 = AH(1, matrix, c);
    Eigen::Matrix4f A_2 = AH(2, matrix, c);
    Eigen::Matrix4f A_3 = AH(3, matrix, c);
    Eigen::Matrix4f A_4 = AH(4, matrix, c);
    Eigen::Matrix4f A_5 = AH(5, matrix, c);
    Eigen::Matrix4f A_6 = AH(6, matrix, c);

    Eigen::Matrix4f A_7 = AH(7, matrix, c);
    Eigen::Matrix4f A_8 = AH(8, matrix, c);
    Eigen::Matrix4f A_9 = AH(9, matrix, c);
    Eigen::Matrix4f A_10 = AH(10, matrix, c);
    Eigen::Matrix4f A_11 = AH(11, matrix, c);
    Eigen::Matrix4f A_12 = AH(12, matrix, c);

    Eigen::Matrix4f A_13 = AH(13, matrix, c);
    Eigen::Matrix4f A_14 = AH(14, matrix, c);
    Eigen::Matrix4f A_15 = AH(15, matrix, c);
    Eigen::Matrix4f A_16 = AH(16, matrix, c);
    Eigen::Matrix4f A_17 = AH(17, matrix, c);
    Eigen::Matrix4f A_18 = AH(18, matrix, c);

    Eigen::Matrix4f A_19 = AH(19, matrix, c);
    Eigen::Matrix4f A_20 = AH(20, matrix, c);
    Eigen::Matrix4f A_21 = AH(21, matrix, c);
    Eigen::Matrix4f A_22 = AH(22, matrix, c);
    Eigen::Matrix4f A_23 = AH(23, matrix, c);
    Eigen::Matrix4f A_24 = AH(24, matrix, c);

    return  A_1  * A_2  * 
            A_3  * A_4  * 
            A_5  * A_6  *
            A_7  * A_8  *
            A_9  * A_10 *
            A_11 * A_12 *
            A_13 * A_14 *
            A_15 * A_16 *
            A_17 * A_18 *
            A_19 * A_20 *
            A_21 * A_22 *
            A_23 * A_24;
}

unsigned long int HTransParallel6(void)
{
    forward_struct settings_ = forward_struct([](iterators* iterators_) -> void {

        if(iterators_->n == 1) 
        {
            A_1 = AH(iterators_->n, iterators_->matrix, iterators_->c);
            A_2 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
        }
        else if(iterators_->n == 3)
        {
           A_3 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_4 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
        }
        else 
        {
           A_5 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_6 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
        }
        

    }, THREAD_NUMBER);

    auto matrix = Eigen::MatrixXf::Random(WIDTH_6, WIDTH_6);

    iterators *iterators_1 = new iterators(1, matrix, 0); 
    iterators *iterators_2 = new iterators(3, matrix, 0);
    iterators *iterators_3 = new iterators(5, matrix, 0); 

    std::pair<forward_struct, iterators*> * p1 = new std::pair<forward_struct, iterators*>(settings_,iterators_1);
    std::pair<forward_struct, iterators*> * p2 = new std::pair<forward_struct, iterators*>(settings_,iterators_2);
    std::pair<forward_struct, iterators*> * p3 = new std::pair<forward_struct, iterators*>(settings_,iterators_3);
  
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

    Eigen::Matrix4f final_matrix = A_1 * A_2 * A_3 * A_4 * A_5 * A_6;

    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after-before).count();
}

unsigned long int HTransParallel12(void)
{
    forward_struct settings_ = forward_struct([](iterators* iterators_) -> void {

        if(iterators_->n == 1) 
        {
            A_1 = AH(iterators_->n, iterators_->matrix, iterators_->c);
            A_2 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
            A_3 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
            A_4 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
        }
        else if(iterators_->n == 5)
        {
           A_5 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_6 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
           A_7 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
           A_8 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
        }
        else 
        {
           A_9 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_10 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
           A_11 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
           A_12 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
        }
        

    }, THREAD_NUMBER);

    auto matrix = Eigen::MatrixXf::Random(WIDTH_12, WIDTH_12);

    iterators *iterators_1 = new iterators(1, matrix, 0); 
    iterators *iterators_2 = new iterators(5, matrix, 0);
    iterators *iterators_3 = new iterators(9, matrix, 0); 

    std::pair<forward_struct, iterators*> * p1 = new std::pair<forward_struct, iterators*>(settings_,iterators_1);
    std::pair<forward_struct, iterators*> * p2 = new std::pair<forward_struct, iterators*>(settings_,iterators_2);
    std::pair<forward_struct, iterators*> * p3 = new std::pair<forward_struct, iterators*>(settings_,iterators_3);
  
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

    Eigen::Matrix4f final_matrix = A_1 * A_2 * A_3 * A_4 * A_5 * A_6 * A_7 * A_8 * A_9 * A_10 * A_11 * A_12;

    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after-before).count();
}

unsigned long int HTransParallel18(void)
{
    forward_struct settings_ = forward_struct([](iterators* iterators_) -> void {

        if(iterators_->n == 1) 
        {
            A_1 = AH(iterators_->n, iterators_->matrix, iterators_->c);
            A_2 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
            A_3 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
            A_4 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
            A_5 = AH(iterators_->n + 4, iterators_->matrix, iterators_->c);
            A_6 = AH(iterators_->n + 5, iterators_->matrix, iterators_->c);
        }
        else if(iterators_->n == 7)
        {
           A_7 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_8 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
           A_9 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
           A_10 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
           A_11 = AH(iterators_->n + 4, iterators_->matrix, iterators_->c);
           A_12 = AH(iterators_->n + 5, iterators_->matrix, iterators_->c);
        }
        else 
        {
           A_13 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_14 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
           A_15 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
           A_16 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
           A_17 = AH(iterators_->n + 4, iterators_->matrix, iterators_->c);
           A_18 = AH(iterators_->n + 5, iterators_->matrix, iterators_->c);
        }
        

    }, THREAD_NUMBER);

    auto matrix = Eigen::MatrixXf::Random(WIDTH_18, WIDTH_18);

    iterators *iterators_1 = new iterators(1, matrix, 0); 
    iterators *iterators_2 = new iterators(7, matrix, 0);
    iterators *iterators_3 = new iterators(13, matrix, 0); 

    std::pair<forward_struct, iterators*> * p1 = new std::pair<forward_struct, iterators*>(settings_,iterators_1);
    std::pair<forward_struct, iterators*> * p2 = new std::pair<forward_struct, iterators*>(settings_,iterators_2);
    std::pair<forward_struct, iterators*> * p3 = new std::pair<forward_struct, iterators*>(settings_,iterators_3);
  
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

    Eigen::Matrix4f final_matrix = A_1 * A_2 * A_3 * A_4 * A_5 * A_6 * A_7 * A_8 * A_9 * A_10 * A_11 * A_12 * A_13 * A_14 * A_15 * A_16 * A_17 * A_18;

    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after-before).count();
}

unsigned long int HTransParallel24(void)
{
    forward_struct settings_ = forward_struct([](iterators* iterators_) -> void {

        if(iterators_->n == 1) 
        {
            A_1 = AH(iterators_->n, iterators_->matrix, iterators_->c);
            A_2 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
            A_3 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
            A_4 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
            A_5 = AH(iterators_->n + 4, iterators_->matrix, iterators_->c);
            A_6 = AH(iterators_->n + 5, iterators_->matrix, iterators_->c);
            A_7 = AH(iterators_->n + 6, iterators_->matrix, iterators_->c);
            A_8 = AH(iterators_->n + 7, iterators_->matrix, iterators_->c);
        }
        else if(iterators_->n == 9)
        {
           A_9 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_10 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
           A_11 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
           A_12 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
           A_13 = AH(iterators_->n + 4, iterators_->matrix, iterators_->c);
           A_14 = AH(iterators_->n + 5, iterators_->matrix, iterators_->c);
           A_15 = AH(iterators_->n + 6, iterators_->matrix, iterators_->c);
           A_16 = AH(iterators_->n + 7, iterators_->matrix, iterators_->c);
        }
        else 
        {
           A_17 = AH(iterators_->n, iterators_->matrix, iterators_->c);
           A_18 = AH(iterators_->n + 1, iterators_->matrix, iterators_->c);
           A_19 = AH(iterators_->n + 2, iterators_->matrix, iterators_->c);
           A_20 = AH(iterators_->n + 3, iterators_->matrix, iterators_->c);
           A_21 = AH(iterators_->n + 4, iterators_->matrix, iterators_->c);
           A_22 = AH(iterators_->n + 5, iterators_->matrix, iterators_->c);
           A_23 = AH(iterators_->n + 6, iterators_->matrix, iterators_->c);
           A_24 = AH(iterators_->n + 7, iterators_->matrix, iterators_->c);
        }
        

    }, THREAD_NUMBER);

    auto matrix = Eigen::MatrixXf::Random(WIDTH_24, WIDTH_24);

    iterators *iterators_1 = new iterators(1, matrix, 0); 
    iterators *iterators_2 = new iterators(9, matrix, 0);
    iterators *iterators_3 = new iterators(17, matrix, 0); 

    std::pair<forward_struct, iterators*> * p1 = new std::pair<forward_struct, iterators*>(settings_,iterators_1);
    std::pair<forward_struct, iterators*> * p2 = new std::pair<forward_struct, iterators*>(settings_,iterators_2);
    std::pair<forward_struct, iterators*> * p3 = new std::pair<forward_struct, iterators*>(settings_,iterators_3);
  
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

    Eigen::Matrix4f final_matrix = A_1 * A_2 * A_3 * A_4 * A_5 * A_6 * A_7 * A_8 * A_9 * A_10 * A_11 * A_12 * A_13 * A_14 * A_15 * A_16 * A_17 * A_18 * A_19 * A_20 * A_21 * A_22 * A_23 * A_24; 

    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after-before).count();
}

unsigned long int HTransSerial6(void)
{
    auto matrix = Eigen::MatrixXf::Random(WIDTH_6, WIDTH_6);

    auto before = std::chrono::high_resolution_clock::now();
    HTrans6(matrix, 0);
    auto after = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count();
}

unsigned long int HTransSerial12(void)
{
    auto matrix = Eigen::MatrixXf::Random(WIDTH_12, WIDTH_12);

    auto before = std::chrono::high_resolution_clock::now();
    HTrans12(matrix, 0);
    auto after = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count();
}

unsigned long int HTransSerial18(void)
{
    auto matrix = Eigen::MatrixXf::Random(WIDTH_18, WIDTH_18);

    auto before = std::chrono::high_resolution_clock::now();
    HTrans18(matrix, 0);
    auto after = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count();
}

unsigned long int HTransSerial24(void)
{
    auto matrix = Eigen::MatrixXf::Random(WIDTH_24, WIDTH_24);

    auto before = std::chrono::high_resolution_clock::now();
    HTrans24(matrix, 0);
    auto after = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count();
}

void calculate(std::vector<unsigned long int>& times, const int & steps, const char * str)
{
    unsigned long int min = times[0];
    unsigned long int max = times[0];

    double average = 0.0;
    int length = times.size();

    for(int i = 0; i < length; i++) {
        if(min > times[i])
            min = times[i];
        if(max < times[i])
            max = times[i];
        
        average += times[i];
    } 

    std::cout << "Forward Kinematics " << str << " - " << steps << " joints" << std::endl
              << "min: " << min << " max: " << max <<  " avg: " << average/length << std::endl << std::endl;

}

void init_thread_pool( int configuration)
{
    pthread_t threads[10];

    struct sched_param  sched_param_;
    sched_param_.sched_priority = 80;

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


    for(int i = 0; i < 10; i++)
    {
        pthread_detach(threads[i]);
    }
}

}

}

#endif