#Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.25.1)

#Set the project name
project(Front_End_Compiler)

#Set the C standard to use
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

link_libraries(Front_End_Compiler PUBLIC FrontEndFunction)

include_directories("${PROJECT_BINARY_DIR}/include")
                            