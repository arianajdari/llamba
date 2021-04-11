#!/usr/bin/env python3

import os
import sys
import pathlib

def read_file(file_string, file_name):
    file_string = file_string.split("\n")

    number_of_inputs  = 0
    number_of_outputs = 0

    ## PROCESS INPUTS
    if file_string[0].find("input"):
        number_of_inputs  = file_string[0].count("matrix")

    if file_string[1].find("output"):
        number_of_outputs = file_string[1].count("matrix")

    if number_of_inputs == 2:
        generate_function_two_inputs(file_string[3:], file_name)
    else:
        print("TO BE IMPLEMENTED")
    
def generate_function_two_inputs(function, file_name):

    class_name = file_name.split(".")[0]

    generated_string = "#ifndef __" + class_name.upper() + "_PARALLEL_HPP__\n#define __" + class_name.upper() + "_PARALLEL_HPP__\n\n#include <vector>\n#include <math.h>\n#include <sched.h>\n#include <pthread.h>\n#include <llamba/iterators/iterators.hpp>\n#include <llamba/settings/settings.hpp>\n#include <llamba/interfaces/base_parallel.hpp>\n\nnamespace llamba\n{\n\nnamespace base\n{\n\ntemplate<typename T>\nvoid * " + file_name.replace(".", "_") + "_pthread(void * iterators__)\n{\n\titerators<T>* iterators_ = (iterators<T>*)iterators__;\n\n\tfor(int i = iterators_->begin; i < iterators_->end; i++)\n\t{\n\t\tfor(int j = 0; j < iterators_->width; j++)\n\t\t{\n\t\t\t"

    generated_string += function[0].replace("--", "").strip().replace("matrix_result", "iterators_->matrix_c[i * iterators_->width + j]").replace("matrix_a", "iterators_->matrix_a[i * iterators_->width + j]").replace("matrix_b", "iterators_->matrix_b[i * iterators_->width + j]") + ";\n\t\t}\n\t}\n\treturn NULL;\n}\n\n"

    generated_string += "template<typename T>\nvoid " + class_name + "_parallel_openmp(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_ )\n{\n\n\tint matrix_delimeter = sqrt(matrix_a_.size());\n\tint boundary = matrix_a_.size() / sqrt(matrix_a_.size());\n\n\t#pragma omp parallel for num_threads(settings_.THREAD_NUMBER)\n\tfor(int i = 0; i < boundary; i++)\n\t{\n\t\tfor(int j = 0; j < boundary; j++)\n\t\t{\n\t\t\t"

    generated_string += function[0].replace("--", "").strip().replace("matrix_result", "matrix_c_[i * matrix_delimeter + j]").replace("matrix_a", "matrix_a_[i * matrix_delimeter + j]").replace("matrix_b", "matrix_b_[i * matrix_delimeter + j]") + ";\n\t\t}\n\t}\n}\n\n"

    generated_string += "template<typename T>\nvoid " + class_name + "_serial(const std::vector<T>& matrix_a_, const std::vector<T>& matrix_b_, std::vector<T>& matrix_c_, const settings & settings_ )\n{\n\n\tint matrix_delimeter = sqrt(matrix_a_.size());\n\tint boundary = matrix_a_.size() / sqrt(matrix_a_.size());\n\n\tfor(int i = 0; i < boundary; i++)\n\t{\n\t\tfor(int j = 0; j < boundary; j++)\n\t\t{\n\t\t\t"
    
    generated_string += function[0].replace("--", "").strip().replace("matrix_result", "matrix_c_[i * matrix_delimeter + j]").replace("matrix_a", "matrix_a_[i * matrix_delimeter + j]").replace("matrix_b", "matrix_b_[i * matrix_delimeter + j]") + ";\n\t\t}\n\t}\n}\n\n"
       
    abs_path = os.path.abspath(__file__)
    length_abs_path = len(abs_path)
    path_to_base = abs_path[:length_abs_path - 32] + "/python/base/base.txt" 

    current_base = open(path_to_base, "r")

    generated_string += current_base.read().replace("multiply_parallel", class_name + "_parallel").replace("multiply_serial", class_name + "_serial")

    write_file = open(abs_path[:length_abs_path - 32] + "/include/llamba/base/" + file_name.replace(".", "_") + ".hpp", "w")
    write_file.seek(0)

    write_file.write(generated_string)

    write_file.close()
    

abs_path = os.path.abspath(__file__)
length_abs_path = len(abs_path)
path_to_functions = abs_path[:length_abs_path - 32] + "/parallel/" 

for path in pathlib.Path(path_to_functions).iterdir():
    if path.is_file():
        current_file = open(path, "r")
        read_file(current_file.read(), current_file.name.split("/")[-1])
        current_file.close()