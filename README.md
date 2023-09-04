Joy1
----

Build|Linux|Windows|Coverity
---|---|---|---
status|[![GitHub CI build status](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml/badge.svg)](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml)|[![AppVeyor CI build status](https://ci.appveyor.com/api/projects/status/github/Wodan58/joy1?branch=master&svg=true)](https://ci.appveyor.com/project/Wodan58/joy1)|[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/14633.svg)](https://scan.coverity.com/projects/wodan58-joy1)

This is the BDW version of [Joy](https://github.com/Wodan58/Joy). The two
versions are drifting apart.

Changes
-------

Changes are documented in the `doc` directory.
It is always possible to extract an up-to-date version of the manual:

    echo '__html_manual.' | build/joy | lynx -stdin

The lynx browser even adds some color.
The original version can be seen [here](https://github.com/Wodan58/joy0).

Build instructions
------------------

Build with the [BDW garbage collector](https://github.com/ivmai/bdwgc):

    cd build
    SOURCE_DATE_EPOCH=1047920271 cmake ..
    cmake --build .

Alternatives
------------

Implementation|Dependencies
---|---
[Joy - NOBDW](https://github.com/Wodan58/Joy)|
[joy1 - BDW](https://github.com/Wodan58/joy1)|[BDW garbage collector](https://github.com/ivmai/bdwgc)
[Moy](https://github.com/Wodan58/Moy)|[BDW garbage collector](https://github.com/ivmai/bdwgc) and [Lex & Yacc](https://sourceforge.net/projects/winflexbison/files/win_flex_bison-latest.zip)
