#
#  Makefile for JOY
#
cmake_minimum_required(VERSION 3.0)
project(Joy VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)
#add_definitions(-DORIGINAL_JOY -DGC_BDW)
add_definitions(-DGC_NOT_DLL -D_CRT_SECURE_NO_WARNINGS -DGC_BDW -DBIT_32)
include_directories(bdwgc/include)
add_subdirectory(bdwgc)
add_executable(joy interp.c scan.c utils.c main.c)
target_link_libraries(joy gc-lib)
