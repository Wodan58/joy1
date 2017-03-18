#
#  Makefile for JOY
#
cmake_minimum_required(VERSION 3.0)
project(Joy)
set(CMAKE_BUILD_TYPE Release)
add_definitions(-DORIGINAL_JOY -DBIT_32 -I.)
add_executable(joy interp.c scan.c utils.c main.c)
target_link_libraries(joy /usr/local/lib/libgc-lib.a)
