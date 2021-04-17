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
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(multiply_parallel_test src/multiply_parallel_test.cpp)
add_executable(sum_parallel_test src/sum_parallel_test.cpp)
add_executable(abstract_parallel_test src/abstract_parallel_test.cpp)
add_executable(red_channel_parallel_test src/red_channel_parallel_test.cpp)
add_executable(green_channel_parallel_test src/green_channel_parallel_test.cpp)
add_executable(blue_channel_parallel_test src/blue_channel_parallel_test.cpp)
add_executable(inverse_parallel_test src/inverse_parallel_test.cpp)
add_executable(adjoint_parallel_test src/adjoint_parallel_test.cpp)
add_executable(determinant_parallel_test src/determinant_parallel_test.cpp)
add_executable(complex_parallel_test src/complex_parallel_test.cpp)

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


install(TARGETS
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
