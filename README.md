Joy1
----

Build|Linux|Windows|Coverity
---|---|---|---
status|[![GitHub CI build status](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml/badge.svg)](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml)|[![AppVeyor CI build status](https://ci.appveyor.com/api/projects/status/github/Wodan58/joy1?branch=master&svg=true)](https://ci.appveyor.com/project/Wodan58/joy1)|[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/14633.svg)](https://scan.coverity.com/projects/wodan58-joy1)

This is the [BDW](https://github.com/ivmai/bdwgc) version of
[Joy](https://github.com/Wodan58/Joy).

Build instructions
------------------

    cd build
    cmake ..
    cmake --build .

Build with MSVC
---------------

After installing bdwgc in the bdwgc-directory use the CMake GUI to uncheck all
boxes and then check the boxes in the lines that start with disable\_ and
check the last one: without\_libatomic\_ops.

    cd build
    cmake ..
    cmake --build . --config Release
    copy Release\joy.exe

Running
-------

There is a copy of usrlib.joy in the build directory.

See also
--------

Implementation|Dependencies
--------------|------------
[42minjoy](https://github.com/Wodan58/42minjoy)|
[joy0](https://github.com/Wodan58/joy0)|
[Joy](https://github.com/Wodan58/Joy)|
[Moy](https://github.com/Wodan58/Moy)|[BDW garbage collector](https://github.com/ivmai/bdwgc) and [Lex & Yacc](https://sourceforge.net/projects/winflexbison/files/win_flex_bison-latest.zip)

Documentation
-------------

[Legacy Docs](https://wodan58.github.io)
