#Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.25.1)

#Set the project name
project(Front_End_Compiler LANGUAGES CXX) 

#Set the CXX standard to use
set(CMAKE_CXX_STANDARD 17) 

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -w")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/test")


add_executable(Front_End_Compiler main.cxx)

add_subdirectory(src)                           

target_link_libraries(Front_End_Compiler PUBLIC SRC)
