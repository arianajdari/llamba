#include "grayscale.hpp"

extern "C" {
    grayscale* grayscale_create() { return new grayscale(); }
    RESULT* grayscale_execute_serial(grayscale* grayscale_, int data_size, int iteration_number) { return grayscale_->execute_serial(data_size, iteration_number); }
    RESULT* grayscale_execute_eigen( grayscale* grayscale_, int data_size, int iteration_number) { return grayscale_->execute_eigen(data_size, iteration_number);  }
    RESULT* grayscale_execute_openmp(grayscale* grayscale_, int data_size, int iteration_number, int thread_number) { return grayscale_->execute_openmp(data_size, iteration_number, thread_number); }
    RESULT* grayscale_execute_llamba(grayscale* grayscale_, int data_size, int iteration_number, int thread_number, int scheduling_strategy, int priority, bool affinity) { return grayscale_->execute_llamba(data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity); }
}