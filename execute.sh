#!/bin/bash
# This script is used to build and execute the program

cmake -B build
cmake --build build
./build/Front_End_Compiler
