Joy1
====

Build|Linux|Coverity
---|---|---
status|[![GitHub CI build status](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml/badge.svg)](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml)|[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/14633.svg)](https://scan.coverity.com/projects/wodan58-joy1)

This is the [BDW](https://github.com/ivmai/bdwgc) version of
[Joy](https://github.com/Wodan58/Joy).

Build instructions
------------------

    rm -rf build
    mkdir build
    cd build
    cmake -G "Unix Makefiles" ..
    cmake --build .

Build with MSVC
---------------

Install BDW in subdirectory gc-8.2.8

    cmake --build . --config Release
    copy Release\joy.exe

Running
-------

    joy -h

gives an overview of available options.

See also
--------

Implementation|Dependencies
--------------|------------
[42minjoy](https://github.com/Wodan58/42minjoy)|
[joy0](https://github.com/Wodan58/joy0)|
[Joy](https://github.com/Wodan58/Joy)|
[Foy](https://github.com/Wodan58/Foy)|[BDW garbage collector](https://github.com/ivmai/bdwgc)
[Moy](https://github.com/Wodan58/Moy)|[BDW garbage collector](https://github.com/ivmai/bdwgc) and [Lex & Yacc](https://sourceforge.net/projects/winflexbison/files/win_flex_bison-latest.zip)

Documentation|
-------------|
[Legacy Docs](https://wodan58.github.io)
[User Manual](https://wodan58.github.io/j09imp.html)
[Comparison (PDF)](https://github.com/Wodan58/HET/blob/master/doc/FIB.pdf)
[Maintenance Manual (PDF)](https://github.com/Wodan58/G3/blob/master/JOP.pdf)
