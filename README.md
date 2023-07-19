Joy1
----

Build|Linux|Windows|Coverity
---|---|---|---
status|[![GitHub CI build status](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml/badge.svg)](https://github.com/Wodan58/joy1/actions/workflows/cmake.yml)|[![AppVeyor CI build status](https://ci.appveyor.com/api/projects/status/github/Wodan58/joy1?branch=master&svg=true)](https://ci.appveyor.com/project/Wodan58/joy1)|[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/14633.svg)](https://scan.coverity.com/projects/wodan58-joy1)

This is the BDW version of [Joy](https://github.com/Wodan58/Joy). The two
versions are drifting apart.

Build instructions
------------------

Build with the [BDW garbage collector](https://github.com/ivmai/bdwgc):

    cd build
    cmake ..
    cmake --build .

Changes
-------

Changes are mentioned in globals.h and documented in the `doc` directory.
Some builtins have been added: `fget`, `getch`, and `sametype`.
It is always possible to extract an up-to-date version of the manual:

    echo '__html_manual.' | ./joy | lynx -stdin

The lynx browser even adds some color.
The original version can be seen [here](https://github.com/Wodan58/joy0).
