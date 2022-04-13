Joy1
----

Build|Linux|Windows|Coverity|Coverage|Codecov|Quality|Alerts
---|---|---|---|---|---|---|---
status|[![Travis CI build status](https://app.travis-ci.com/Wodan58/joy1.svg?branch=master)](https://app.travis-ci.com/Wodan58/joy1)|[![AppVeyor CI build status](https://ci.appveyor.com/api/projects/status/github/Wodan58/joy1?branch=master&svg=true)](https://ci.appveyor.com/project/Wodan58/joy1)|[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/14633.svg)](https://scan.coverity.com/projects/wodan58-joy1)|[![Coverage Status](https://coveralls.io/repos/github/Wodan58/joy1/badge.svg?branch=master)](https://coveralls.io/github/Wodan58/joy1?branch=master)|[![Codecov](https://codecov.io/gh/Wodan58/joy1/branch/master/graph/badge.svg)](https://codecov.io/gh/Wodan58/joy1)|[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Wodan58/joy1.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Wodan58/joy1/context:cpp)|[![Alerts](https://img.shields.io/lgtm/alerts/g/Wodan58/joy1.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Wodan58/joy1/alerts)

This is the BDW version of [Joy](https://github.com/Wodan58/Joy). The two
versions are drifting apart.

Changes
-------

Changes are mentioned in globals.h and documented in the `doc` directory.
Some builtins have been added: `fget`, `getch`, and `sametype`. Some have been
removed: `__dump`, `__memoryindex`, and `__memorymax`.
It is always possible to extract an up-to-date version of the manual:

    echo '__html_manual.' | ./joy | lynx -stdin

The lynx browser even adds some color.
The original version can be seen [here](https://github.com/alekar/joy).

Installation
------------

Build with the [BDW garbage collector](https://github.com/ivmai/bdwgc), or
without:

    mkdir build
    cd build
    cmake ..
    cmake --build .
