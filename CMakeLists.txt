#
#   module  : CMakeLists.txt
#   version : 1.1
#   date    : 01/14/21
#
cmake_minimum_required(VERSION 3.0)
project(Joy VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)
option(REAL_BDW "Use BDW garbage collector" OFF)
find_library(BDWGC gc)
if(WIN32)
add_definitions(-DGC_NOT_DLL -D_CRT_SECURE_NO_WARNINGS)
else()
#add_definitions(-DTRACING -DSTATS)
add_definitions(-DSTATS)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage") # debug, no optimization
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage") # enabling coverage
endif()
if(REAL_BDW)
add_definitions(-DGC_BDW)
include_directories(bdwgc/include)
add_subdirectory(bdwgc)
add_executable(joy interp.c scan.c utils.c main.c srand.c rand.c)
target_link_libraries(joy "${BDWGC}" m)
else()
include_directories(.)
add_executable(joy interp.c scan.c utils.c main.c gc.c srand.c rand.c)
target_link_libraries(joy m)
endif()
if(WIN32)
else()
add_subdirectory(lib)
add_subdirectory(test)
endif()
