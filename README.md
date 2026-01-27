Joy1
====

Build|Linux
---|---
status|[![GitHub CI build status](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml/badge.svg)](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml)

This is the [BDW](https://github.com/ivmai/bdwgc) version of
[Joy](https://github.com/Wodan58/Joy).

Build instructions
------------------

    cd build
    cmake -G "Unix Makefiles" ..
    cmake --build .

Build with MSVC
---------------

Install BDW in (sibling)directory gc-8.2.10

    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build .

Running
-------

    joy -h

gives an overview of available options.

See also
--------

Implementation|Dependencies
--------------|------------
[42minjoy](https://github.com/Wodan58/42minjoy)|Minimal Joy
[joy0](https://github.com/Wodan58/joy0)|Original Joy
[Joy](https://github.com/Wodan58/Joy)|NOBDW Joy
[Foy](https://github.com/Wodan58/Foy)|[BDW garbage collector](https://github.com/ivmai/bdwgc)
[Moy](https://github.com/Wodan58/Moy)|[BDW garbage collector](https://github.com/ivmai/bdwgc) and [Lex & Yacc](https://sourceforge.net/projects/winflexbison/files/win_flex_bison-latest.zip)

Documentation|Notes
-------------|-----
[Legacy Docs](https://wodan58.github.io)|Original Joy documentation
[User Manual](https://wodan58.github.io/j09imp.html)|Joy language reference
[Comparison](https://github.com/Wodan58/HET/blob/master/doc/FIB.pdf)|Fibonacci
[Main Manual](https://github.com/Wodan58/G3/blob/master/JOP.pdf)|Comprehensive Joy manual
