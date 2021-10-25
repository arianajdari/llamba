cmake_minimum_required(VERSION 3.5)
project(llamba)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(OpenMP REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(include)


## LAMBA COMPARED WITH OTHER APPROACHES ##

######################## MULTIPLICATION BENCHMARK #######################################
add_executable(01_multiplication_benchmark src/parallelization_component/01_multiplication_benchmark.cpp)
target_link_libraries(01_multiplication_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(01_multiplication_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################## MULTIPLICATION BENCHMARK #######################################

######################### SUMMATION BENCHMARK ###########################################
add_executable(02_summation_benchmark src/parallelization_component/02_summation_benchmark.cpp)
target_link_libraries(02_summation_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(02_summation_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### SUMMATION BENCHMARK ###########################################

######################### BRESENHAM BENCHMARK ###########################################
add_executable(03_bresenham_benchmark src/parallelization_component/03_bresenham_benchmark.cpp)
target_link_libraries(03_bresenham_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(03_bresenham_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### BRESENHAM BENCHMARK ###########################################

######################### STATICMAP BENCHMARK ###########################################
add_executable(04_staticmap_benchmark src/parallelization_component/04_staticmap_benchmark.cpp)
target_link_libraries(04_staticmap_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(04_staticmap_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### STATICMAP BENCHMARK ###########################################

######################### FORWARD BENCHMARK #############################################
add_executable(05_forward_benchmark src/parallelization_component/05_forward_benchmark.cpp)
target_link_libraries(05_forward_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(05_forward_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### FORWARD BENCHMARK #############################################

######################### GRAYSCALE BENCHMARK ###########################################
add_executable(06_grayscale_benchmark src/parallelization_component/06_grayscale_benchmark.cpp)
target_link_libraries(06_grayscale_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(06_grayscale_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### GRAYSCALE BENCHMARK ###########################################

######################### BLUE CHANNEL BENCHMARK ########################################
add_executable(07_blue_channel_extraction_benchmark src/parallelization_component/07_blue_channel_extraction_benchmark.cpp)
target_link_libraries(07_blue_channel_extraction_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(07_blue_channel_extraction_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### BLUE CHANNEL BENCHMARK ########################################


## LLAMBA'S BENCHMARK COMPONENT ##

######################### MULTIPLICATION LLAMBA BENCHMARK ###############################
add_executable(01_multiplication_benchmark_llamba src/benchmark_component/01_multiplication_benchmark_llamba.cpp)
target_link_libraries(01_multiplication_benchmark_llamba   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(01_multiplication_benchmark_llamba PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### MULTIPLICATION LLAMBA BENCHMARK ###############################

######################### MULTIPLICATION LLAMBA BENCHMARK ###############################
add_executable(02_summation_benchmark_llamba src/benchmark_component/02_summation_benchmark_llamba.cpp)
target_link_libraries(02_summation_benchmark_llamba   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(02_summation_benchmark_llamba PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### MULTIPLICATION LLAMBA BENCHMARK ###############################

######################### MULTIPLICATION LLAMBA BENCHMARK ###############################
add_executable(03_grayscale_benchmark_llamba src/benchmark_component/03_grayscale_benchmark_llamba.cpp)
target_link_libraries(03_grayscale_benchmark_llamba   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(03_grayscale_benchmark_llamba PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### MULTIPLICATION LLAMBA BENCHMARK ###############################

######################### MULTIPLICATION LLAMBA BENCHMARK ###############################
add_executable(04_blue_channel_extraction_benchmark_llamba src/benchmark_component/04_blue_channel_extraction_benchmark_llamba.cpp)
target_link_libraries(04_blue_channel_extraction_benchmark_llamba   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(04_blue_channel_extraction_benchmark_llamba PUBLIC  "${OpenMP_CXX_FLAGS}")
######################### MULTIPLICATION LLAMBA BENCHMARK ###############################

install(TARGETS
  01_multiplication_benchmark
  02_summation_benchmark
  03_bresenham_benchmark
  04_staticmap_benchmark
  05_forward_benchmark
  06_grayscale_benchmark
  07_blue_channel_extraction_benchmark

  01_multiplication_benchmark_llamba
  02_summation_benchmark_llamba
  03_grayscale_benchmark_llamba
  04_blue_channel_extraction_benchmark_llamba

  DESTINATION lib/${PROJECT_NAME}
)