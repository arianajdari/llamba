#ifndef __BASE_PARALLEL_HPP__
#define __BASE_PARALLEL_HPP__

#include <vector>
#include <string>

#include <llamba/settings/settings.hpp>

namespace llamba
{

template<typename T>
class benchmark;

template<typename T>
class base_parallel
{
    protected:
        virtual void ctor(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) = 0;
    private:

        virtual std::string get_name() const = 0;

        virtual void execute(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) = 0;
        virtual void execute_pthread(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) = 0;
        virtual void execute_pthread_affine(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) = 0;
        virtual void execute_pthread_sched(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) = 0;
        virtual void execute_pthread_affine_sched(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) = 0;
        //virtual void execute_openmp(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_ ) = 0;

        friend class benchmark<T>;
};

}

#endif