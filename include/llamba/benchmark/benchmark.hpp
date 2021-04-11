#ifndef __BENCHMARK_HPP__
#define __BENCHMARK_HPP__

#include <string>
#include <vector>
#include <chrono>
#include <iostream>

#include <llamba/settings/settings.hpp>
#include <llamba/settings/time_format.hpp>
#include <llamba/interfaces/base_parallel.hpp>
#include <llamba/generators/json_generator.hpp>

namespace llamba
{

template<typename T>
class benchmark
{
    public:


        benchmark(base_parallel<T>* base_parallel_, std::vector<T> input_a_, std::vector<T> input_b_, settings settings_) : 
            base_parallel_(base_parallel_),
            input_a_(input_a_),
            input_b_(input_b_),
            settings_(settings_)
        {
            result_.reserve(input_a_.capacity());
        }

        benchmark(base_parallel<T>* base_parallel_, std::vector<std::vector<T> > inputs_a_, std::vector<std::vector<T> > inputs_b_, std::vector<settings> settingss_) :
            base_parallel_(base_parallel_),
            inputs_a_(inputs_a_),
            inputs_b_(inputs_b_),
            settingss_(settingss_)
        {
            
        }

        template<typename K = TIME_FORMAT::NANO> 
        unsigned long long
        execute()
        {
            before = std::chrono::high_resolution_clock::now();
            base_parallel_->ctor(input_a_, input_b_, result_, settings_);
            after  = std::chrono::high_resolution_clock::now();

            return std::chrono::duration_cast<std::chrono::duration<int64_t, K> >(after - before).count();
        }

        template<typename K = TIME_FORMAT::NANO>
        void
        execute_all()
        {

            std::string name = base_parallel_->get_name();
            
            json_generator_.add("{\n\t\"name\" : \"")
                           .add(name)
                           .add("\",\n\t\"time_unit\" : \"")
                           .add<K>(std::chrono::duration<int64_t, K>())
                           .add("\",")
                           .add("\n\t\"tests\" : {");

            for(int i = 0; i < settingss_.size(); i++)
            {
                std::cout << "Running Test " << i + 1 << " from " << settingss_.size() << std::endl;

                json_generator_.add("\n\t\t\"test_")
                               .add(i + 1)
                               .add("\" : {")
                               .add("\n\t\t\t\"settings\" : ")
                               .add(settingss_[i])
                               .add(",\n\t\t\t\"measurements\" : {");

                for(int j = 0; j < inputs_a_.size(); j++)
                {
                    result_.reserve(inputs_a_[j].capacity());
                    before = std::chrono::high_resolution_clock::now();
                    base_parallel_->ctor(inputs_a_[j], inputs_b_[j], result_, settingss_[i]);
                    after  = std::chrono::high_resolution_clock::now();

                    json_generator_.add("\n\t\t\t\t\"")
                                   .add((int&&)sqrt(inputs_a_[j].size()))
                                   .add("x")
                                   .add((int&&)sqrt(inputs_a_[j].size()))
                                   .add("\" : ")
                                   .add((int&&)std::chrono::duration_cast<std::chrono::duration<int64_t, K> >(after - before).count());

                    if(j != inputs_a_.size() - 1)
                        json_generator_.add(",");
                   
                    result_.clear();
                }

                json_generator_.add("\n\t\t\t}");

                if(i == settingss_.size() - 1)
                    json_generator_.add("\n\t\t}");
                else
                    json_generator_.add("\n\t\t},");

            }

            json_generator_.add("\n\t}\n}");

            json_generator_.save_to_json_file(base_parallel_->get_name());

            std::cout << "Done!" << std::endl;
        }

        template<typename K = TIME_FORMAT::NANO>
        void
        execute_all(std::string file_path)
        {

            std::string name = base_parallel_->get_name();
            
            json_generator_.add("{\n\t\"name\" : \"")
                           .add(name)
                           .add("\",\n\t\"time_unit\" : \"")
                           .add<K>(std::chrono::duration<int64_t, K>())
                           .add("\",")
                           .add("\n\t\"tests\" : {");

            for(int i = 0; i < settingss_.size(); i++)
            {
                std::cout << "Running Test " << i + 1 << " from " << settingss_.size() << std::endl;

                json_generator_.add("\n\t\t\"test_")
                               .add(i + 1)
                               .add("\" : {")
                               .add("\n\t\t\t\"settings\" : ")
                               .add(settingss_[i])
                               .add(",\n\t\t\t\"measurements\" : {");

                for(int j = 0; j < inputs_a_.size(); j++)
                {
                    result_.reserve(inputs_a_[j].capacity());
                    before = std::chrono::high_resolution_clock::now();
                    base_parallel_->ctor(inputs_a_[j], inputs_b_[j], result_, settingss_[i]);
                    after  = std::chrono::high_resolution_clock::now();

                    json_generator_.add("\n\t\t\t\t\"")
                                   .add((int&&)sqrt(inputs_a_[j].size()))
                                   .add("x")
                                   .add((int&&)sqrt(inputs_a_[j].size()))
                                   .add("\" : ")
                                   .add((int&&)std::chrono::duration_cast<std::chrono::duration<int64_t, K> >(after - before).count());

                    if(j != inputs_a_.size() - 1)
                        json_generator_.add(",");
                   
                    result_.clear();
                }

                json_generator_.add("\n\t\t\t}");

                if(i == settingss_.size() - 1)
                    json_generator_.add("\n\t\t}");
                else
                    json_generator_.add("\n\t\t},");

            }

            json_generator_.add("\n\t}\n}");

            json_generator_.save_to_json_file(file_path + base_parallel_->get_name());

            std::cout << "Done!" << std::endl;
        }


       
    private:
    
        std::chrono::high_resolution_clock::time_point before;
        std::chrono::high_resolution_clock::time_point after;

        base_parallel<T>* base_parallel_;

        std::vector<T> input_a_;
        std::vector<T> input_b_;
        std::vector<T> result_;
        settings settings_;

        std::vector<std::vector<T> > inputs_a_;
        std::vector<std::vector<T> > inputs_b_;
        
        std::vector<std::vector<T>> results_;
        std::vector<settings>       settingss_;

        json_generator json_generator_;
        
};

}

#endif