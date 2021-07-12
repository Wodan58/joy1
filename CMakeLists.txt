#
#   module  : CMakeLists.txt
#   version : 1.5
#   date    : 06/28/21
#
cmake_minimum_required(VERSION 3.0)
project(Joy VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)
option(REAL_BDW "Use BDW garbage collector" OFF)
if(WIN32)
add_definitions(-DGC_NOT_DLL -D_CRT_SECURE_NO_WARNINGS)
else()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage") # debug, no optimization
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage") # enabling coverage
endif()
if(REAL_BDW)
add_definitions(-DGC_BDW)
include_directories(bdwgc/include)
add_executable(joy interp.c scan.c utils.c main.c srand.c rand.c factor.c)
if(WIN32)
target_link_libraries(joy gc-lib)
else()
target_link_libraries(joy gc-lib m)
endif()
add_subdirectory(bdwgc)
else()
add_executable(joy interp.c scan.c utils.c main.c srand.c rand.c factor.c gc.c)
if(WIN32)
else()
target_link_libraries(joy m)
endif()
endif()
add_dependencies(joy always)
add_custom_target(always
		  COMMAND sh banner.sh ${PROJECT_NAME} ${PROJECT_VERSION}
		  COMMAND sh table.sh
		  COMMAND sh prims.sh
		 )
add_subdirectory(lib)
add_subdirectory(test)
