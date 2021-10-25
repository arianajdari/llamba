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
#include <llamba/base/multiply_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#include "Eigen/Dense"

#define THREAD_POOL 4
#define THREAD_NUMBER_ 4

#define DATA_SIZE 800
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

struct multiply
{
    void (*fn) (iterators*);
    int thread_number;
    bool is_finished;
    
    multiply(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false) {};
};


std::pair<multiply, iterators*>* a[THREAD_POOL];


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

unsigned long int multiplication()
{
    multiply settings_ = multiply([](iterators* iterators_) -> void {

        for(int i = iterators_->begin; i < iterators_->end; i++)
        {
            for(int j = 0; j < iterators_->width; j++)
            {
                for(int k = 0; k < iterators_->width; k++)
                {
                    iterators_->matrix_c[i * iterators_->width + j] += 
                        iterators_->matrix_a[i * iterators_->width + k] * 
                        iterators_->matrix_b[k * iterators_->width + j]; 
                }
            }
        }
        
    }, THREAD_NUMBER_);

    auto matrix_a_ = create_vector(DATA_SIZE * DATA_SIZE);

    auto matrix_b_ = create_vector(DATA_SIZE * DATA_SIZE);
    std::vector<int> matrix_c_(DATA_SIZE * DATA_SIZE);

    std::pair<multiply, iterators*> * k[settings_.thread_number];



   
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

        std::pair<multiply, iterators*> * p = new std::pair<multiply, iterators*>(settings_,iterators_);

        
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


void calculate(std::vector<unsigned long int>& times, const int & steps, const char * str)
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

    std::cout << "Multiplication " << str << " - " << steps << " steps" << std::endl
              << "min: " << min << " max: " << max <<  " avg: " << average/length << std::endl << std::endl;

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

unsigned long int standard_deviation(const std::vector<unsigned long int>& values)
{
    double average_ = average(values);

    double standard_deviation = 0.0;

    for(int i = 0; i < values.size(); i++)
    {
        standard_deviation += pow(average_ - values[i], 2);
    }

    standard_deviation /= ITERATION_NUMBER;

    return sqrt(standard_deviation);
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
         
       //pthread_setschedparam(*(threads + i), 0, &sched_param_);
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


    std::vector<unsigned long int> serial_times;
    std::vector<unsigned long int> eigen_times;
    std::vector<unsigned long int> llamba_times;
    std::vector<unsigned long int> llamba_threadpool_times;
    std::vector<unsigned long int> openmp_times;

    Eigen::MatrixXd a = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);
    Eigen::MatrixXd b = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);

    auto matrix_a  = llamba::single_generator::generate_input<double>(DATA_SIZE);
    auto matrix_b  = llamba::single_generator::generate_input<double>(DATA_SIZE);
    llamba::settings settings_ = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);
    llamba::settings settings_openmp = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::OPENMP);
    llamba::settings settings_serial = llamba::settings(PARALLELIZATION_STRATEGY::SERIAL);

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_openmp);
        auto after = std::chrono::high_resolution_clock::now();
        openmp_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_serial);
        auto after = std::chrono::high_resolution_clock::now();
        serial_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }
    
    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto result  = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);
        auto before = std::chrono::high_resolution_clock::now();
        llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_);
        auto after = std::chrono::high_resolution_clock::now();
        llamba_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }


    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        auto before = std::chrono::high_resolution_clock::now();
        Eigen::MatrixXd c = a * b;
        auto after = std::chrono::high_resolution_clock::now();
        eigen_times.push_back(std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT > >(after - before).count());
    }

 

    for(int i = 0; i < ITERATION_NUMBER; i++)
    {
        llamba_threadpool_times.push_back(multiplication());
    }

    std::cout << "Multiplication Benchmark -- Matrix Size: (" << DATA_SIZE << "x" << DATA_SIZE << ")" << std::endl << std::endl;

    
    calculate(serial_times, ITERATION_NUMBER, "Serial");
    calculate(llamba_times, ITERATION_NUMBER, "LLAMBA");
    calculate(eigen_times, ITERATION_NUMBER, "Eigen");
    calculate(openmp_times, ITERATION_NUMBER, "OpenMP");
    calculate(llamba_threadpool_times, ITERATION_NUMBER, "LLAMBA");


/*
    std::cout << "Serial  times after " << ITERATION_NUMBER << " iterations (average): " << average(serial_times) << "ms" << std::endl;
    std::cout << "Llamba  times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_times) << "ms" << std::endl;
    std::cout << "Eigen                  " << ITERATION_NUMBER << " iterations (average): " << average(eigen_times) << "ms" << std::endl;
    std::cout << "OpenMP                 " << ITERATION_NUMBER << " iterations (average): " << average(openmp_times) << "ms" << std::endl;
    std::cout << "Llamba PTH times after " << ITERATION_NUMBER << " iterations (average): " << average(llamba_threadpool_times) << "ms" << std::endl;
*/

    std::cout << std::endl;



    std::cout << std::endl << "Press ctrl+c to exit the program...";

    while(true) {}

    return 0;
}