#
#  Makefile for JOY
#
cmake_minimum_required(VERSION 2.8.9)
project(Joy)

set(GC_BDW ON)
set(BIT_32 ON)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,0x100000")

if(GC_BDW)
  add_definitions(-DGC_BDW -DSINGLE -DSTATS)
endif()

add_definitions(-I../bdwgc/include -O3 -Wall -Wextra -Werror)

if(BIT_32)
  add_definitions(-DBIT_32 -ansi -pedantic)
else()
  add_definitions(-Wno-format)
endif()

add_executable(joy interp.c scan.c utils.c main.c)

if(GC_BDW)
  add_library(gc STATIC IMPORTED)
  set_target_properties(gc PROPERTIES IMPORTED_LOCATION ../bdwgc/libgc-lib.a)
  target_link_libraries(joy gc)
endif()
