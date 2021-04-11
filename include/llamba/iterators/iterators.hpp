#ifndef __ITERATORS_HPP__
#define __ITERATORS_HPP__

#include <vector>

namespace llamba
{

template<typename T>
struct iterators
{

    iterators(
        const std::vector<T>& matrix_a_, 
        const std::vector<T>& matrix_b_,
        std::vector<T>& matrix_c_, 
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

    const std::vector<T>& matrix_a;
    const std::vector<T>& matrix_b;
    std::vector<T>& matrix_c;
    int begin;
    int end;
    int size;
    int width;
};

}

#endif