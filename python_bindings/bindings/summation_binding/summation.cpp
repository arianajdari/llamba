#include "summation.hpp"

extern "C" {
    summation* summation_create() { return new summation(); }
    RESULT* summation_execute_serial(summation* summation_, int data_size, int iteration_number) { return summation_->execute_serial(data_size, iteration_number); }
    RESULT* summation_execute_eigen( summation* summation_, int data_size, int iteration_number) { return summation_->execute_eigen(data_size, iteration_number);  }
    RESULT* summation_execute_openmp(summation* summation_, int data_size, int iteration_number, int thread_number) { return summation_->execute_openmp(data_size, iteration_number, thread_number); }
    RESULT* summation_execute_llamba(summation* summation_, int data_size, int iteration_number, int thread_number, int scheduling_strategy, int priority, bool affinity) { return summation_->execute_llamba(data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity); }
}