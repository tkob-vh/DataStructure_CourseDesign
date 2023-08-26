#Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.25.1)

#Set the project name
project(Front_End_Compiler)

#Set the CXX standard to use
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

add_executable(Front_End_Compiler main.cxx)

target_link_libraries(Front_End_Compiler PUBLIC FrontEndFunction)

target_include_directories(Front_End_Compiler PUBLIC "${PROJECT_BINARY_DIR}/include")
                            

