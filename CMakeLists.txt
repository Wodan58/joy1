#
#  Makefile for JOY
#
cmake_minimum_required(VERSION 2.8.9)
project(Joy)
add_definitions(-DGC_BDW -DBIT_64 -DSINGLE -I.)
add_executable(joy interp.c scan.c utils.c main.c)
target_link_libraries(joy /usr/local/lib/libgc-lib.a)
