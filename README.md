Joy1
====

Build|Linux
-----|-----
status|[![GitHub CI build status](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml/badge.svg)](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml)

This is a simplified version of [Joy](https://github.com/Wodan58/Joy).

Build instructions
------------------

    cd build
    cmake -G "Unix Makefiles" ..
    cmake --build .

Build with MSVC
---------------

    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build .

Running
-------

    joy -h

gives an overview of available options.

See also
--------

Implementation|Notes
--------------|-----
[42minjoy](https://github.com/Wodan58/42minjoy)|Minimal Joy
[joy0](https://github.com/Wodan58/joy0)|Original Joy
[Joy](https://github.com/Wodan58/Joy)|Copying GC Joy
[Foy](https://github.com/Wodan58/Foy)|FORTH inspired Joy
[Moy](https://github.com/Wodan58/Moy)|Stackless Joy

Documentation|Notes
-------------|-----
[Legacy Docs](https://wodan58.github.io)|Original Joy documentation
[User Manual](https://wodan58.github.io/j09imp.html)|Joy language reference
[Main Manual](https://github.com/Wodan58/G3/blob/master/JOP.pdf)|Comprehensive Joy manual
