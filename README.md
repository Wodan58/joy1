Joy1
----

Build|Linux|Windows|Coverity|Quality|Coverage|Codecov|Alerts
---|---|---|---|---|---|---|---
status|[![Travis CI build status](https://travis-ci.org/Wodan58/joy1.svg?branch=master)](https://travis-ci.org/Wodan58/joy1)|[![AppVeyor CI build status](https://ci.appveyor.com/api/projects/status/github/Wodan58/joy1?branch=master&svg=true)](https://ci.appveyor.com/project/Wodan58/joy1)|[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/14633.svg)](https://scan.coverity.com/projects/wodan58-joy1)|[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Wodan58/joy1.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Wodan58/joy1/context:cpp)|[![Coverage Status](https://coveralls.io/repos/github/Wodan58/joy1/badge.svg?branch=master)](https://coveralls.io/github/Wodan58/joy1?branch=master)|[![Codecov](https://codecov.io/gh/Wodan58/joy1/branch/master/graph/badge.svg)](https://codecov.io/gh/Wodan58/joy1)|[![Alerts](https://img.shields.io/lgtm/alerts/g/Wodan58/joy1.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Wodan58/joy1/alerts)

Changes
-------

Changes are documented in globals.h. The original version is available [here](https://github.com/Wodan58/Joy).

Installation
------------

Build instructions are available in appveyor.yml or .travis.yml or .lgtm.yml.
Alternatively, omakefile or nmakefile from [Joy](https://github.com/Wodan58/Joy)
can be used. Then:

make -f omakefile

or:

nmake -f nmakefile

will build joy(.exe)

The [BDW garbage collector](https://github.com/ivmai/bdwgc) needs to be
installed and compiled separately, but this is optional.
