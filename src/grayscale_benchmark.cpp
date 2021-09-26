#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <thread>
#include <mutex>
#include <sched.h>
#include <llamba/base/grayscale_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include "Eigen/Dense"

#define DATA_TYPE int
#define DATA_SIZE 2000

#define THREAD_POOL 4
#define THREAD_NUMBER_ 4

#define ITERATION_NUMBER 5
typedef std::milli TIME_MEASUREMENT;


std::mutex mutexes[THREAD_POOL];


struct iterators
{
    iterators(
        const std::vector<int>& matrix_a_, 
        const std::vector<int>& matrix_b_,
        std::vector<int>& matrix_c_, 
        int begin_,
        int end_,
        int size_, 
        int width_) : 
        matrix_a(matrix_a_),
        matrix_b(matrix_b_),
        matrix_c(matrix_c_),
        begin(begin_),
        end(end_),
        size(size_),
        width(width_)
    {

    }

    const std::vector<int>& matrix_a;
    const std::vector<int>& matrix_b;
    std::vector<int>& matrix_c;
    int begin;
    int end;
    int size;
    int width;
};

struct grayscale
{
    void (*fn) (iterators*);
    int thread_number;
    bool is_finished;
    
    grayscale(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false) {};
};


std::pair<grayscale, iterators*>* a[THREAD_POOL];


void * pool_thread_worker(void * arg__)
{
    int* arg = (int*) arg__;

    while(true)
    {
        if(mutexes[*arg].try_lock()) 
        {
            if(a[*arg] != NULL)
            {
                auto p = a[*arg];
                if(!p->first.is_finished)
                {
                    p->first.fn(p->second);
                    p->first.is_finished = true;   
                }
            }
            mutexes[*arg].unlock();
        }
    }

    return NULL;
}


void * check_if_not_null(void * arg__)
{
    while(true)
    {
        
        for(int i = 0; i < THREAD_POOL; i++)
        {
            if(a[i] != NULL && a[i]->first.is_finished)
            {
                mutexes[i].lock();
                a[i] = NULL;
                mutexes[i].unlock();
                
            }
        }
        
    }
}


std::vector<int> create_vector(int width)
{
    std::vector<int> a(width);

    for(int i = 0; i < width; i++)
    {
        a[i] = i + 1;
    }

    return a;
}

unsigned long int grayscale_pool_thread()
{
    grayscale settings_ = grayscale([](iterators* iterators_) -> void {

        for(int i = iterators_->begin; i < iterators_->end; i++)
        {
            for(int j = 0; j < iterators_->width; j++)
            {
                iterators_->matrix_c[i * iterators_->width + j] = ( ((iterators_->matrix_a[i * iterators_->width + j])&0x000000FF) + ((iterators_->matrix_a[i * iterators_->width + j] >> 8)&0x000000FF) + ((iterators_->matrix_a[i * iterators_->width + j] >> 16)&0x000000FF) ) / 3;
            }
        }
        
    }, THREAD_NUMBER_);

    auto matrix_a_ = create_vector(DATA_SIZE * DATA_SIZE);
    auto matrix_b_ = create_vector(DATA_SIZE * DATA_SIZE);
    std::vector<int> matrix_c_(DATA_SIZE * DATA_SIZE);

    std::pair<grayscale, iterators*> * k[settings_.thread_number];



   
    for(int i = 0; i < settings_.thread_number; i++)
    {
        iterators *iterators_;
        if(i != settings_.thread_number - 1)
        {
            int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.thread_number);
            iterators_ = new iterators(
                matrix_a_, matrix_b_, matrix_c_,
                i * delimeter,
                (i + 1) * delimeter,
                matrix_a_.size(),
                (int)(sqrt(matrix_a_.size()))
            );
        }
        else
        {
            int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.thread_number);
            iterators_ = new iterators(
                matrix_a_, matrix_b_, matrix_c_,
                i * delimeter,
                matrix_a_.size() / sqrt(matrix_a_.size()),
                matrix_a_.size(),
                (int)(sqrt(matrix_a_.size()))
            );
        }

        std::pair<grayscale, iterators*> * p = new std::pair<grayscale, iterators*>(settings_,iterators_);

        
       k[i] = p;
        
    }

    auto before = std::chrono::high_resolution_clock::now();
    a[0] = k[0];
    a[1] = k[1];
    a[2] = k[2];
    a[3] = k[3];

    while(true) 
    {
        if( a[0] == NULL && 
            a[1] == NULL && 
            a[2] == NULL &&
            a[3] == NULL)
        {
            break;
        }
    }
    auto after = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count();
}

void grayscale_eigen(Eigen::VectorXi & vec) 
{
    for(int i = 0; i < DATA_SIZE * DATA_SIZE; i++)
    {
        vec(i) = ((vec(i)&0x000000FF) + ((vec(i) >> 8 )&0x000000FF) + ((vec(i) >> 16 )&0x000000FF)) / 3;
    }
}

unsigned long int average(const std::vector<unsigned long int>& values)
{
    double average = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        average += values[i];
    }

    return average / ITERATION_NUMBER;
}

int main()
{   
    cpu_set_t cpuset;
    int cpu_counter = 0;
    struct sched_param  sched_param_;
    sched_param_.sched_priority = 80;

    pthread_t threads[THREAD_POOL];

    for(int i = 0; i < THREAD_POOL; i++)
    {
        CPU_ZERO(&cpuset);
        int* arg = new int;
        *arg = i;
        a[i] = NULL;
        CPU_SET(cpu_counter, &cpuset);
        
        pthread_create(&threads[i], NULL, pool_thread_worker, (void*)arg);
         
        pthread_setschedparam(*(threads + i), 0, &sched_param_);
        pthread_setaffinity_np(*(threads + i), sizeof(cpuset), &cpuset);
        cpu_counter += 2;
    }

    for(int i = 0; i < THREAD_POOL; i++)
    {
        pthread_detach(threads[i]);
    }

    pthread_t thread_check;
    pthread_create(&thread_check, NULL, check_if_not_null, NULL);
    pthread_detach(thread_check);


    

    std::vector<unsigned long int> eigen_times;

    std::vector<unsigned long int> serial_times;
    std::vector<unsigned long int> llamba_times_standard;
    std::vector<unsigned long int> llamba_times_affinity;
    std::vector<unsigned long int> llamba_times_priority;
    std::vector<unsigned long int> llamba_times_affinity_priority;

    std::vector<unsigned long int> llamba_threadpool_times;
    std::vector<unsigned long int> openmp_times;

    auto matrix_a  = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);
    auto matrix_b  = llamba::single_generator::generate_input<DATA_TYPE>(DATA_SIZE);

    Eigen::VectorXi vec(DATA_SIZE * DATA_SIZE);

    for(int i = 0; i < DATA_SIZE * DATA_SIZE; i++)
    {
        vec(i) = i;
    }
    
    llamba::settings settings_serial            = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);
    llamba::settings settings_standard          = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 19_PRIORITY, AFFINITY::FALSE, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_affinity          = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 19_PRIORITY, AFFINITY::TRUE, std::vector<int>{0, 2, 4, 6}, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_priority          = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 80_PRIORITY, AFFINITY::FALSE, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_affinity_priority = llamba::settings(4_THREADS, SCHEDULING_STRATEGY::SCHEDULING_TS, 80_PRIORITY, AFFINITY::TRUE, std::vector<int>{0, 2, 4, 6}, PARALLELIZATION_STRATEGY::PTHREADS);
    
    llamba::settings settings_openmp            = llamba::settings(PARALLELIZATION_STRATEGY::OPENMP);

    std::cout << "Starting serial..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_serial);
        auto after = std::chrono::high_resolution_clock::now();
        serial_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Standard..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_standard);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_standard.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Affinity..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_affinity);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_affinity.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Priority..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_priority);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_priority.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA Affinity and Priority..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_affinity_priority);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times_affinity_priority.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting OpenMP..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<DATA_TYPE>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::grayscale_parallel<DATA_TYPE> grayscale = llamba::base::grayscale_parallel<DATA_TYPE>(matrix_a, matrix_b, result, settings_openmp);
        auto after = std::chrono::high_resolution_clock::now();
        openmp_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    std::cout << "Done" << std::endl;

    std::cout << "Starting LLAMBA ThreadPool..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        llamba_threadpool_times.push_back(grayscale_pool_thread());
    }

    std::cout << "Done" << std::endl;

    std::cout << "Starting Eigen..." << std::flush;
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto before = std::chrono::high_resolution_clock::now();
        grayscale_eigen(vec);
        auto after = std::chrono::high_resolution_clock::now();
        eigen_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    std::cout << "Done" << std::endl << std::endl;


    std::cout << "Grayscale Benchmark -- Matrix Size: (" << DATA_SIZE << "x" << DATA_SIZE << ")" << std::endl << std::endl;
    
    std::cout << "Serial                       times after " << ITERATION_NUMBER << " iterations (average): " << average(serial_times)                   << "ms" << std::endl;
    std::cout << "Llamba Standard              times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_times_standard)          << "ms" << std::endl;
    std::cout << "Llamba Priority              times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_times_priority)          << "ms" << std::endl;
    std::cout << "Llamba Affinity              times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_times_affinity)          << "ms" << std::endl;
    std::cout << "Llamba Priority and Affinity times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_times_affinity_priority) << "ms" << std::endl;
    std::cout << "OpenMP                       times after " << ITERATION_NUMBER << " iterations (average): " << average(openmp_times)                   << "ms" << std::endl;
    std::cout << "Llamba Pool Thread           times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_threadpool_times)        << "ms" << std::endl;
    std::cout << "Eigen                        times after " << ITERATION_NUMBER << " iterations (average): " << average(eigen_times)                    << "ms" << std::endl;

    std::cout << std::endl;

    std::cout << std::endl << "Press ctrl+c to exit the program...";

    while(true) {}

    return 0;
}