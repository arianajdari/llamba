#include "bresenham.hpp"

extern "C" {
    bresenham* bresenham_create(int llamba_configuration) { return new bresenham(llamba_configuration); }
    RESULT* bresenham_execute_serial(bresenham* bresenham_, int iteration_number, int step_size) { return bresenham_->execute_serial(iteration_number, step_size); }
    RESULT* bresenham_execute_llamba(bresenham* bresenham_, int iteration_number, int step_size) { return bresenham_->execute_llamba(iteration_number, step_size); }
}