#ifndef __JSON_GENERATOR_HPP__
#define __JSON_GENERATOR_HPP__

#include <string>
#include <chrono>
#include <sstream>
#include <fstream>
#include <iostream>

#include <llamba/settings/settings.hpp>
#include <llamba/settings/time_format.hpp>

namespace llamba
{

class json_generator
{
    public:

        json_generator() = default;

        json_generator& add(const settings& settings_)
        {
            output_stream << settings_;

            return *this;
        }

        json_generator& add(const int&& iteration_)
        {
            output_stream << iteration_;

            return *this;
        }

        json_generator& add(std::string text_stream)
        {
            output_stream << text_stream.c_str();

            return *this;
        }

        template<typename T = TIME_FORMAT::NANO>
        json_generator& add(const std::chrono::duration<int64_t, T>& duration)
        {
            if(T::den / 1000000000 == 1)
                output_stream << "nano";
            else if(T::den / 1000000 == 1)
                output_stream << "micro";
            else
                output_stream << "milli";
            return *this;
        }

        void save_to_json_file(const std::string& file_name)
        {
            std::ofstream ofs(file_name + ".json", std::ios::out);

            ofs << output_stream.str();

            ofs.close();
        }

        void print()
        {
            std::cout << output_stream.str() << std::endl;
        }

    private:
        std::stringstream output_stream;
};

}

#endif