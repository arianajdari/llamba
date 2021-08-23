#ifndef __SINGLE_GENERATOR_HPP__
#define __SINGLE_GENERATOR_HPP__

#include <vector>
#include <time.h>
#include <stdlib.h>

namespace llamba
{

class single_generator
{
    public:
        template<typename T = int>
        static std::vector<T> generate_input(int size)
        {
            std::vector<T> temp(size * size);

            srand(time(NULL));

            for(int i = 0; i < temp.capacity(); i++)
            {
                temp[i] = (T)(((rand() % RAND_MAX) + 1)+(rand() / RAND_MAX));
            }

            return temp;
        }

        template<typename T = int>
        static std::vector<T> generate_input_zero(int size)
        {
            std::vector<T> temp(size * size);

            return temp;
        }
};

}

#endif