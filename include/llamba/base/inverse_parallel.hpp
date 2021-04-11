#ifndef __INVERSE_PARALLEL_HPP__
#define __INVERSE_PARALLEL_HPP__

#include <vector>
#include <math.h>
#include <sched.h>
#include <pthread.h>
#include <type_traits>
#include <functional>

#include <llamba/iterators/iterators.hpp>
#include <llamba/settings/settings.hpp>
#include <llamba/interfaces/base_parallel.hpp>

#include <llamba/base/adjoint_parallel.hpp>


namespace llamba
{

namespace base
{

template<typename T>
class inverse_parallel : public base_parallel<T>
{
    public:

        inverse_parallel() : name("inverse_parallel")
        {
            
        }

        inverse_parallel(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) : name("inverse_parallel")
        {
            matrix_c_.reserve(matrix_a_.size());
            this->execute(matrix_a_, matrix_b_, matrix_c_, settings_);
        }

        inverse_parallel(const std::vector<T>& matrix_a_, std::vector<T>& matrix_c_, const settings & settings_) : name("inverse_parallel")
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
            std::vector<T> determinant(1);

            std::vector<T> adjoint(matrix_a_.size());

            //determinant_parallel determinant_parallel_(matrix_a_, matrix_b_, determinant, settings(PARALLELIZATION_STRATEGY::SERIAL));

            adjoint_parallel<T> adjoint_parallel_(matrix_a_, matrix_b_, adjoint, settings_);

            int matrix_delimeter = sqrt(matrix_a_.size());

            for (int i=0; i<matrix_delimeter; i++)
                for (int j=0; j<matrix_delimeter; j++)
                    matrix_c_[i * matrix_delimeter + j] = adjoint[i * matrix_delimeter + j]/float(1);


            
        }

        void execute_pthread(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) { }

        void execute_pthread_affine(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) { }

        void execute_pthread_sched(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) { }

        void execute_pthread_affine_sched(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_) { }

};

}

}

#endif