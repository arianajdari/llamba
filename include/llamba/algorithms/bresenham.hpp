#ifndef __BRESENHAM_HPP__
#define __BRESENHAM_HPP__

#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <utility>
#include <vector>
#include <math.h>
#include <sched.h>
#include <omp.h>


#define THREAD_POOL 5
#define THREAD_NUMBER 4

typedef std::milli TIME_MEASUREMENT;

namespace llamba
{

namespace algorithm
{

void Bresenham3D(int x1, int y1, int z1, const int & x2, const int & y2, const int & z2){
    
    int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    int point[3];
    
    point[0] = x1;
    point[1] = y1;
    point[2] = z1;
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    x_inc = (dx < 0) ? -1 : 1;
    l = abs(dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = abs(dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;


   
    
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point[0] += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point[1] += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point[2] += z_inc;
        }
    }
}

struct iterators
{
  iterators(int x1, int y1, int z1, int  x2,  int  y2,  int  z2) :
    x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2)  
  {

  }

  int x1;
  int y1;
  int z1;
  int x2;
  int y2;
  int z2;
};

struct bresenham_struct
{
  void (*fn) (iterators*);
  int thread_number;
  bool is_finished;
  bool is_processing;
  
  bresenham_struct(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false), is_processing(false) {};
};

std::pair<bresenham_struct, iterators*>* tasks[THREAD_POOL];

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


unsigned long int bresenham_multithreading(int x1, int y1, int z1, const int & x2, const int & y2, const int & z2)
{
  bresenham_struct settings_ = bresenham_struct([](iterators* iterators_) -> void {


    int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    int point[3];

   
   int 
    x1 = iterators_->x1,
    x2 = iterators_->x2,
    y1 = iterators_->y1,
    y2 = iterators_->y2,
    z1 = iterators_->z1,
    z2 = iterators_->z2;

    
    point[0] = x1;
    point[1] = y1;
    point[2] = z1;
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    x_inc = (dx < 0) ? -1 : 1;
    l = abs(dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = abs(dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;
    
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point[0] += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point[1] += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point[2] += z_inc;
        }
    }        
  }, THREAD_NUMBER);

  
  iterators *iterators_1 = new iterators(0, y1, z1, x2 / 4, y2, z2); 
  iterators *iterators_2 = new iterators(x2 / 4, y1, z1, (x2 / 4) * 2, y2, z2);
  iterators *iterators_3 = new iterators((x2 / 4) * 2, y1, z1, (x2 / 4) * 3, y2, z2); 
  iterators *iterators_4 = new iterators((x2 / 4) * 3, y1, z1, x2, y2, z2); 

  std::pair<bresenham_struct, iterators*> * p1 = new std::pair<bresenham_struct, iterators*>(settings_,iterators_1);
  std::pair<bresenham_struct, iterators*> * p2 = new std::pair<bresenham_struct, iterators*>(settings_,iterators_2);
  std::pair<bresenham_struct, iterators*> * p3 = new std::pair<bresenham_struct, iterators*>(settings_,iterators_3);
  std::pair<bresenham_struct, iterators*> * p4 = new std::pair<bresenham_struct, iterators*>(settings_,iterators_4);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  tasks[3] = p4;

 
  
  
  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished && tasks[3]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after-before).count();
}


double bresenham_single_threaded(int x1, int y1, int z1, const int & x2, const int & y2, const int & z2) 
{
   // double itime = omp_get_wtime();

    auto before = std::chrono::high_resolution_clock::now();
    Bresenham3D(x1,y1,z1, x2, y2, z2);
    auto after = std::chrono::high_resolution_clock::now();
   //double ftime = omp_get_wtime();

    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after-before).count();
}

void calculate(std::vector<double>& times, const int & steps, const char * str)
{
    double min = times[0];
    double max = times[0];

    double average = 0.0;
    int length = times.size();

    for(int i = 0; i < length; i++) {
        if(min > times[i])
            min = times[i];
        if(max < times[i])
            max = times[i];
        
        average += times[i];
    } 

    std::cout << "Bresenham " << str << " - " << steps << " steps" << std::endl
              << "min: " << min << " max: " << max <<  " avg: " << average/length << std::endl << std::endl;

}

void init_thread_pool(int configuration)
{
    struct sched_param  sched_param_;
    sched_param_.sched_priority = 80;

    cpu_set_t cpuset;
    int cpu_counter = 0;
    
    
    pthread_t threads[THREAD_POOL];

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

    for(int i = 0; i < THREAD_POOL; i++)
    {
        pthread_detach(threads[i]);
    }
}

}

}

#endif

