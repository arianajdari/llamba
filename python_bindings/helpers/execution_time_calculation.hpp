#ifndef ___EXECUTION_TIME_CALCULATION_HPP___
#define ___EXECUTION_TIME_CALCULATION_HPP___

#include <vector>

#include "return_container.hpp"

class execution_time_calculation {
    protected:
        std::vector<int> create_vector(int width)
        {
            std::vector<int> a(width);

            for(int i = 0; i < width; i++)
            {
                a[i] = i + 1;
            }

            return a;
        }

        RESULT* calculate(std::vector<unsigned long int>& times, const int & data_size)
        {
            RESULT* result = new RESULT();

            double min = times[0];
            double max = times[0];

            double avg = 0.0;
            int length = times.size();

            for(int i = 0; i < length; i++) {
                if(min > times[i])
                    min = times[i];
                if(max < times[i])
                    max = times[i];
                
                avg += times[i];
            } 

            result->data_size = data_size;
            result->iteration_number = length;
            result->min = min;
            result->max = max;
            result->avg = avg / length;

            return result;
        }
};

#endif