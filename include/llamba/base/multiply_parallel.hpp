#ifndef __MULTIPLY_PARALLEL_HPP__
#define __MULTIPLY_PARALLEL_HPP__

#include <vector>
#include <math.h>
#include <sched.h>
#include <pthread.h>

#include <llamba/iterators/iterators.hpp>
#include <llamba/settings/settings.hpp>
#include <llamba/interfaces/base_parallel.hpp>


namespace llamba
{

namespace base
{

template<typename T>
void * multiply_parallel_pthread(void * iterators__)
{
    iterators<T>* iterators_ = (iterators<T>*)iterators__;

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
    return NULL;   
}

template<typename T>
void multiply_parallel_openmp(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_ )
{

    int matrix_delimeter = sqrt(matrix_a_.size());
    int boundary = matrix_a_.size() / sqrt(matrix_a_.size());

    #pragma omp parallel for num_threads(settings_.THREAD_NUMBER) collapse(3)
    for(int i = 0; i < boundary; i++)
    {
        for(int j = 0; j < boundary; j++)
        {
            for(int k = 0; k < boundary; k++)
            {
                matrix_c_[i * matrix_delimeter + j] += 
                    matrix_a_[i * matrix_delimeter + k] * 
                    matrix_b_[k * matrix_delimeter + j]; 
            }
        }
    }
}

template<typename T>
void multiply_serial(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_)
{
    int matrix_delimeter = sqrt(matrix_a_.size());
    int boundary = matrix_a_.size() / sqrt(matrix_a_.size());

    for(int i = 0; i < boundary; i++)
    {
        for(int j = 0; j < boundary; j++)
        {
            for(int k = 0; k < boundary; k++)
            {
                matrix_c_[i * matrix_delimeter + j] += 
                    matrix_a_[i * matrix_delimeter + k] * 
                    matrix_b_[k * matrix_delimeter + j]; 
            }
        }
    }
}


template<typename T>
class multiply_parallel : public base_parallel<T>
{
    public:

        multiply_parallel() : name("multiply_parallel")
        {
            
        }

        multiply_parallel(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) : name("multiply_parallel")
        {
            matrix_c_.reserve(matrix_a_.size());
            this->execute(matrix_a_, matrix_b_, matrix_c_, settings_);
        }

        multiply_parallel(const std::vector<T>& matrix_a_, std::vector<T>& matrix_c_, const settings & settings_) : name("multiply_parallel")
        {
            matrix_c_.reserve(matrix_a_.size() / 2);

            const std::vector<T> matrix_one{matrix_a_.begin(), (matrix_a_.size() / 2) + matrix_a_.begin()};
            const std::vector<T> matrix_two{(matrix_a_.size() / 2) + matrix_a_.begin(), matrix_a_.end()};
            this->execute(matrix_one, matrix_two, matrix_c_, settings_);
        }

    protected:
        void ctor(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) 
        {
            matrix_c_.reserve(matrix_a_.size());
            this->execute(matrix_a_, matrix_b_, matrix_c_, settings_);
        }        

    private:

        std::string name;

        std::string get_name() const
        {
            return this->name;
        }

        void execute(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_)
        {
            if(settings_.PARALLELIZATION_STRATEGY == PARALLELIZATION_STRATEGY::PTHREADS)
            {
                if(settings_.AFFINITY == AFFINITY::TRUE)
                {
                    if(settings_.SCHED_STRATEGY != SCHEDULING_STRATEGY::SCHEDULING_TS)
                        execute_pthread_affine_sched(matrix_a_,matrix_b_, matrix_c_, settings_);
                    else
                        execute_pthread_affine(matrix_a_,matrix_b_, matrix_c_, settings_);
                }
                else
                {
                    if(settings_.SCHED_STRATEGY != SCHEDULING_STRATEGY::SCHEDULING_TS)
                        execute_pthread_sched(matrix_a_,matrix_b_, matrix_c_, settings_);
                    else
                        execute_pthread(matrix_a_,matrix_b_, matrix_c_, settings_);
                }

            }
            else if(settings_.PARALLELIZATION_STRATEGY == PARALLELIZATION_STRATEGY::OPENMP)
            {
                multiply_parallel_openmp(matrix_a_,matrix_b_, matrix_c_, settings_);
            }
            else 
            {
                multiply_serial(matrix_a_,matrix_b_, matrix_c_);
            }
            
        }

        void execute_pthread(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_)
        {
            pthread_t* threads = new pthread_t[settings_.THREAD_NUMBER];
            iterators<T>** iterators_ = new iterators<T>*[settings_.THREAD_NUMBER];

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                if(i != settings_.THREAD_NUMBER - 1)
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        (i + 1) * delimeter,
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                else
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        matrix_a_.size() / sqrt(matrix_a_.size()),
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                pthread_create(threads + i, NULL, multiply_parallel_pthread<T>, (void*)iterators_[i]);                  
            }

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                pthread_join(*(threads + i), NULL);
            }
        }

        void execute_pthread_affine(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_)
        {
            pthread_t* threads = new pthread_t[settings_.THREAD_NUMBER];
            iterators<T>** iterators_ = new iterators<T>*[settings_.THREAD_NUMBER];

            cpu_set_t cpuset;
            int cpu_counter = 0;
           

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {   
                CPU_ZERO(&cpuset);
                if(i != settings_.THREAD_NUMBER - 1)
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        (i + 1) * delimeter,
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                else
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        matrix_a_.size() / sqrt(matrix_a_.size()),
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                pthread_create(threads + i, NULL, multiply_parallel_pthread<T>, (void*)iterators_[i]); 
                CPU_SET(cpu_counter, &cpuset);
                pthread_setaffinity_np(*(threads + i), sizeof(cpuset), &cpuset);
                
                if(settings_.LOGICAL_CPUS.size() > 0) 
                {
                    CPU_CLR(settings_.LOGICAL_CPUS[cpu_counter], &cpuset);
                    cpu_counter = (cpu_counter + 1) % settings_.LOGICAL_CPUS.size();     
                }
                else
                {
                    CPU_CLR(cpu_counter, &cpuset);
                    cpu_counter += 2;    
                }
                        
            }

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                pthread_join(*(threads + i), NULL);
            }
        }

        void execute_pthread_sched(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_)
        {
            pthread_t* threads = new pthread_t[settings_.THREAD_NUMBER];
            iterators<T>** iterators_ = new iterators<T>*[settings_.THREAD_NUMBER];

            struct sched_param  sched_param_;
            sched_param_.sched_priority = settings_.PRIORITY;

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                if(i != settings_.THREAD_NUMBER - 1)
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        (i + 1) * delimeter,
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                else
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        matrix_a_.size() / sqrt(matrix_a_.size()),
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                pthread_create(threads + i, NULL, multiply_parallel_pthread<T>, (void*)iterators_[i]);  
                pthread_setschedparam(*(threads + i), settings_.SCHED_STRATEGY, &sched_param_);                
            }

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                pthread_join(*(threads + i), NULL);
            }
        }

        void execute_pthread_affine_sched(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_)
        {
            pthread_t* threads = new pthread_t[settings_.THREAD_NUMBER];
            iterators<T>** iterators_ = new iterators<T>*[settings_.THREAD_NUMBER];
            
            cpu_set_t cpuset;
            int cpu_counter = 0;

            struct sched_param  sched_param_;
            sched_param_.sched_priority = settings_.PRIORITY;

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                CPU_ZERO(&cpuset);
                if(i != settings_.THREAD_NUMBER - 1)
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        (i + 1) * delimeter,
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                else
                {
                    int delimeter = (int)((matrix_a_.size() / sqrt(matrix_a_.size())) / settings_.THREAD_NUMBER);
                    iterators_[i] = new iterators<T>(
                        matrix_a_, matrix_b_, matrix_c_,
                        i * delimeter,
                        matrix_a_.size() / sqrt(matrix_a_.size()),
                        matrix_a_.size(),
                        (int)(sqrt(matrix_a_.size()))
                    );
                }
                pthread_create(threads + i, NULL, multiply_parallel_pthread<T>, (void*)iterators_[i]); 

                CPU_SET(cpu_counter, &cpuset);
                pthread_setaffinity_np(*(threads + i), sizeof(cpuset), &cpuset); 
                
                pthread_setschedparam(*(threads + i), settings_.SCHED_STRATEGY, &sched_param_);  
                
                cpu_counter += 2;    
                CPU_CLR(cpu_counter, &cpuset);                      
            }

            for(int i = 0; i < settings_.THREAD_NUMBER; i++)
            {
                pthread_join(*(threads + i), NULL);
            }
        }

};

}

}

#endif