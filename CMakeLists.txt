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

add_executable(main src/main.cpp)

target_include_directories(main PUBLIC include)
target_link_libraries(main   "${OpenMP_CXX_FLAGS}")
target_compile_options(main PUBLIC  "${OpenMP_CXX_FLAGS}")


install(TARGETS
  main
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
