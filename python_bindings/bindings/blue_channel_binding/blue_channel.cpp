#include "blue_channel.hpp"

extern "C" {
    blue_channel* blue_channel_create() { return new blue_channel(); }
    RESULT* blue_channel_execute_serial(blue_channel* blue_channel_, int data_size, int iteration_number) { return blue_channel_->execute_serial(data_size, iteration_number); }
    RESULT* blue_channel_execute_eigen( blue_channel* blue_channel_, int data_size, int iteration_number) { return blue_channel_->execute_eigen(data_size, iteration_number);  }
    RESULT* blue_channel_execute_openmp(blue_channel* blue_channel_, int data_size, int iteration_number, int thread_number) { return blue_channel_->execute_openmp(data_size, iteration_number, thread_number); }
    RESULT* blue_channel_execute_llamba(blue_channel* blue_channel_, int data_size, int iteration_number, int thread_number, int scheduling_strategy, int priority, bool affinity) { return blue_channel_->execute_llamba(data_size, iteration_number, thread_number, scheduling_strategy, priority, affinity); }
}