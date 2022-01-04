#include "multiplication.hpp"

extern "C" {
    multiplication* multiplication_create() { return new multiplication(); }
    RESULT* multiplication_execute_serial(multiplication* multiplication_, int data_size, int iteration_number) { return multiplication_->execute_serial(data_size, iteration_number); }
    RESULT* multiplication_execute_eigen( multiplication* multiplication_, int data_size, int iteration_number) { return multiplication_->execute_eigen(data_size, iteration_number);  }
    RESULT* multiplication_execute_openmp(multiplication* multiplication_, int data_size, int iteration_number, int thread_number) { return multiplication_->execute_openmp(data_size, iteration_number, thread_number); }
    RESULT* multiplication_execute_llamba(multiplication* multiplication_, int data_size, int iteration_number, int thread_number, int scheduling_strategy, int priority, bool affinity) { return multiplication_->execute_llamba(data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity); }
}