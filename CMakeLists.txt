#
#  Makefile for JOY
#
cmake_minimum_required(VERSION 3.0)
project(Joy VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)
add_definitions(-DGC_NOT_DLL -D_CRT_SECURE_NO_WARNINGS -DGC_BDW -DBIT_32)
if(WIN32)
else()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage") # debug, no optimization
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage") # enabling coverage
endif()
include_directories(bdwgc/include)
add_subdirectory(bdwgc)
add_executable(joy interp.c scan.c utils.c main.c)
target_link_libraries(joy gc-lib)
add_custom_target(demo01.txt ALL
		  DEPENDS joy
		  COMMAND joy gcd 24 54 >demo01.txt)
add_custom_target(demo02.txt ALL
		  DEPENDS joy
		  COMMAND joy joytut.inp >demo02.txt)
add_custom_target(demo03.txt ALL
		  DEPENDS joy
		  COMMAND joy lsptst.joy >demo03.txt)
add_custom_target(demo04.txt ALL
		  DEPENDS joy
		  COMMAND joy jp-joytst.joy >demo04.txt)
add_custom_target(demo05.txt ALL
		  DEPENDS joy
		  COMMAND joy laztst.joy >demo05.txt)
add_custom_target(demo06.txt ALL
		  DEPENDS joy
		  COMMAND joy grmtst.joy >demo06.txt)
add_custom_target(demo07.txt ALL
		  DEPENDS joy
		  COMMAND joy modtst.joy >demo07.txt)
add_custom_target(demo08.txt ALL
		  DEPENDS joy
		  COMMAND joy mtrtst.joy >demo08.txt)
add_custom_target(demo09.txt ALL
		  DEPENDS joy
		  COMMAND joy plgtst.joy >demo09.txt)
add_custom_target(demo10.txt ALL
		  DEPENDS joy
		  COMMAND joy symtst.joy >demo10.txt)
add_custom_target(demo11.txt ALL
		  DEPENDS joy
		  COMMAND joy test.joy >demo11.txt)
add_custom_target(demo12.txt ALL
		  DEPENDS joy
		  COMMAND joy reptst.joy >demo12.txt)
add_custom_target(demo13.txt ALL
		  DEPENDS joy
		  COMMAND joy jp-nestrec.joy >demo13.txt)
add_custom_target(demo14.txt ALL
		  DEPENDS joy
		  COMMAND joy jp-reprodtst.joy >demo14.txt)
add_custom_target(demo15.txt ALL
		  DEPENDS joy
		  COMMAND joy flatjoy.joy >demo15.txt)
add_custom_target(demo16.txt ALL
		  DEPENDS joy
		  COMMAND joy jp-church.joy >demo16.txt)
