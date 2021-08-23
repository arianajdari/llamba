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

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(multiplication_benchmark src/multiplication_benchmark.cpp)
add_executable(summation_benchmark src/summation_benchmark.cpp)
add_executable(abstraction_benchmark src/abstraction_benchmark.cpp)
add_executable(ros_benchmarks src/ros_benchmarks.cpp)
add_executable(bresenham_benchmark src/bresenham_benchmark.cpp)
add_executable(odometry_benchmark src/odometry_benchmark.cpp)
add_executable(staticmap_benchmark src/odometry_benchmark.cpp)
add_executable(forward_benchmark src/forward_benchmark.cpp)
add_executable(inversion_benchmark src/inversion_benchmark.cpp)

add_executable(multiply_parallel_test tests/multiply_parallel_test.cpp)
add_executable(sum_parallel_test tests/sum_parallel_test.cpp)
add_executable(abstract_parallel_test tests/abstract_parallel_test.cpp)
add_executable(red_channel_parallel_test tests/red_channel_parallel_test.cpp)
add_executable(green_channel_parallel_test tests/green_channel_parallel_test.cpp)
add_executable(blue_channel_parallel_test tests/blue_channel_parallel_test.cpp)
add_executable(inverse_parallel_test tests/inverse_parallel_test.cpp)
add_executable(adjoint_parallel_test tests/adjoint_parallel_test.cpp)
add_executable(determinant_parallel_test tests/determinant_parallel_test.cpp)
add_executable(complex_parallel_test tests/complex_parallel_test.cpp)
add_executable(grayscale_parallel_test tests/grayscale_parallel_test.cpp)

target_include_directories(multiply_parallel_test PUBLIC include)
target_link_libraries(multiply_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(multiply_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(sum_parallel_test PUBLIC include)
target_link_libraries(sum_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(sum_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(abstract_parallel_test PUBLIC include)
target_link_libraries(abstract_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(abstract_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(red_channel_parallel_test PUBLIC include)
target_link_libraries(red_channel_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(red_channel_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(green_channel_parallel_test PUBLIC include)
target_link_libraries(green_channel_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(green_channel_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(blue_channel_parallel_test PUBLIC include)
target_link_libraries(blue_channel_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(blue_channel_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(inverse_parallel_test PUBLIC include)
target_link_libraries(inverse_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(inverse_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(adjoint_parallel_test PUBLIC include)
target_link_libraries(adjoint_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(adjoint_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(determinant_parallel_test PUBLIC include)
target_link_libraries(determinant_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(determinant_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(complex_parallel_test PUBLIC include)
target_link_libraries(complex_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(complex_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(grayscale_parallel_test PUBLIC include)
target_link_libraries(grayscale_parallel_test   "${OpenMP_CXX_FLAGS}")
target_compile_options(grayscale_parallel_test PUBLIC  "${OpenMP_CXX_FLAGS}")


target_include_directories(multiplication_benchmark PUBLIC include)
target_link_libraries(multiplication_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(multiplication_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(summation_benchmark PUBLIC include)
target_link_libraries(summation_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(summation_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(abstraction_benchmark PUBLIC include)
target_link_libraries(abstraction_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(abstraction_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(bresenham_benchmark PUBLIC include)
target_link_libraries(bresenham_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(bresenham_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(odometry_benchmark PUBLIC include)
target_link_libraries(odometry_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(odometry_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(staticmap_benchmark PUBLIC include)
target_link_libraries(staticmap_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(staticmap_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(forward_benchmark PUBLIC include)
target_link_libraries(forward_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(forward_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(inversion_benchmark PUBLIC include)
target_link_libraries(inversion_benchmark   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(inversion_benchmark PUBLIC  "${OpenMP_CXX_FLAGS}")

target_include_directories(ros_benchmarks PUBLIC include)
target_link_libraries(ros_benchmarks   "${OpenMP_CXX_FLAGS}" Eigen3::Eigen)
target_compile_options(ros_benchmarks PUBLIC  "${OpenMP_CXX_FLAGS}")
ament_target_dependencies(ros_benchmarks rclcpp std_msgs)


install(TARGETS
  ros_benchmarks
  multiplication_benchmark
  sum_parallel_test
  multiply_parallel_test
  abstract_parallel_test
  red_channel_parallel_test
  green_channel_parallel_test
  blue_channel_parallel_test
  inverse_parallel_test
  adjoint_parallel_test
  determinant_parallel_test
  complex_parallel_test
  grayscale_parallel_test
  summation_benchmark
  abstraction_benchmark
  bresenham_benchmark
  odometry_benchmark
  staticmap_benchmark
  forward_benchmark
  inversion_benchmark
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_include_directories(include)

add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "include/")

install(
  DIRECTORY include/ ${CMAKE_CURRENT_BINARY_DIR}/include/
  DESTINATION include
)

ament_python_install_package(${PROJECT_NAME})

install(PROGRAMS
  scripts/hardware_information.py
  scripts/generate_visuals.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
