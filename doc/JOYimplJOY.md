 ![](Wynn.PNG)
==============

Introduction
============

Changes that are mentioned in globals.h are described here. A summary of the
changes is at the end of this page.

Recent changes
==============

The macro NEWNODE was added to globals.h, because of unspecified behaviour.

READ_PRIVATE_AHEAD
------------------

This macro surrounds the code that enables local symbols and also public member
functions to call each other. This feature is not supported by all versions of
Joy and thus should be considered implementation dependent. Also, there is an
easy workaround: insert an empty forward declaration.

SEARCH_ARGV0_DIRECTORY
----------------------

This macro was added in order to support out-of-source builds with Cmake. It
can be helpful during operating Joy as well: Joy loads `usrlib.joy` and searches
this library in the current directory. With this macro enabled Joy can locate
`usrlib.joy` also in another directory: the same directory as used by `argv[0]`.

Accepted changes
================

Changes that have been accepted, because they corrected simple mistakes are not
mentioned here. They are still present in globals.h, because these changes are
sometimes undone or moved from accepted to not accepted or someting like that.

FGET_FROM_FILE
--------------

There are a number of builtins that read from stdin; the same number of
builtins should be available for reading from a different file descriptor.
`fget` fills part of that gap.

GETCH_AS_BUILTIN
----------------

There are a number of file operations that send data to output, for example
`putch`. So, why not have the same number of operations on input? `getch` fills
that gap. It could be defined as: `stdin fgetch popd`.

SAMETYPE_BUILTIN
----------------

The predicate `sametype` was lost during the development of Joy and is a useful
addition. It allows some datatype specific predicates to be replaced by
`sametype`, if needed.

CORRECT_TYPE_COMPARE, CORRECT_CASE_COMPARE
------------------------------------------

It would be good to have only one definition of equality. `Compare` compares
each type with every other type and is a robust way to enforce the same kind of
equality in `compare,equal,case,in,has,=,<` and other comparison operators. But
`in` has a problem: it breaks `grmtst.joy`. This was finally solved by using a
nickname: plus and ast instead of + and *.

ONLY_LOGICAL_NOT
----------------

It looks ok to have `not` only available for BOOLEAN_ and SET_. After all,
`not` inverts a value and while it is possible to change every other value into
0, inverting 0 is only possible if there is only 1 value to invert to. Also, if
`not` is available for other datatypes, it overlaps functionality with `null`.

CORRECT_FREAD_PARAM
-------------------

The builtin `fread` throws away the file descriptor it reads from and that is
different from the documentation. This looks like a mistake, but correcting it
might break existing programs.

CORRECT_INTERN_LOOKUP
---------------------

The builtin `intern` allows interning of symbols with spaces or other
characters that do not adhere to the naming restrictions of identifiers. This
looks like a mistake, but it is not necessary to have this corrected. After
all, there exists a different way of accessing symbols: `"symbol" intern ==
[symbol] first`. In both cases, the symbol `symbol` will be placed on the
stack. There are 3 exceptions to the equality of `intern` and `[ .. ] first`:
`false`, `true`, and `maxint`. Thus: `"false" intern != [false] first`. The
word `intern` returns the function `false`, whereas the construct with `first`
returns the value `false`.

REST_OF_UNIX_ESCAPES
--------------------

Character escape sequences are easier to remember when this is enabled: all
ASCII values between 8 and 13 inclusive can then be escaped in a symbolic way.
Not that there is an urgent need for this addition, because escaping can also
be done numerically: '\n is equal to '\010.

CORRECT_ALEN
------------

The longest identifier in the sources is 21 characters long. Including the
terminating 0, a buffer of 22 characters is needed. On the other hand, if
identifiers are going to be stored in a database, the shorter definition of 20
might be preferable. And no matter what value is given to ALEN, there will
always be identifiers that are longer.

NO_HELP_LOCAL_SYMBOLS
---------------------

Symbols from the PRIVATE section of a module or symbols that are defined
between HIDE and IN end up in the symbol table. This change prevents them from
showing up in the symbol table. This also means that the output of `alljoy.joy`
will be different when this change is enabled. Not having local symbols shown
seems good to have, because they cannot be used outside their respective PUBLIC
sections. This is now accepted and not present in the source anymore.

USE_UNKNOWN_SYMBOLS
-------------------

This definition makes it possible that local symbols can call each other.
The only way to make it a 100% solution is to read the PRIVATE sections twice:
during the first read defined symbols are registered in the symbol table and
during the second read normal processing is done. The benefit of this
approach is that symbols, because they can be made local, do not show up in
the symbol table. There is an other approach that requires no change: simply
use forward declarations in the PRIVATE sections. For some implementations of
the symbol table this might be easier to implement.

CORRECT_INHAS_COMPARE
---------------------

This definition would break `grmtst.joy` and that is not allowed.
All comparison operators now use the Compare function and a redefinition of
`in` in `grmtst.joy` is no longer necessary.

Existing changes
================

Some existing functionality has been guarded with defines.

USE_SHELL_ESCAPE
----------------

This is existing behaviour of Joy, that is surrounded by this definition. There
might be a security issue here.

ENABLE_TRACEGC
--------------

Likewise, existing behaviour. It reports on memory allocations in the NOBDW
version.

RUNTIME_CHECKS, NCHECK
----------------------

Likewise, existing behaviour. It is not recommended to turn this off, because
it takes very little time to check parameters and it is a good thing to have,
catching bugs when running a program. The new name is now NCHECK with reverse
meaning. NCHECK looks line NDEBUG, but NDEBUG is turned on automatically by
Cmake in the Release version; turning off or on should be a deliberate decision.

Unaccepted changes
==================

The changes listed here may have had their use at some point in time, but are
now considered not needed.

TRACK_USED_SYMBOLS
------------------

This definition uses the program to generate a list of symbols that are used in
the Joy source files; it can be used to detect what the largest length of a
symbol is.

NOT_ALSO_FOR_FLOAT, NOT_ALSO_FOR_FILE
-------------------------------------

The builtin `not` was generic, but not defined for all datatypes. In hindsight,
it looks better to restrict the use of `not` to logicals and that makes these
definitions obsolete.

CHECK_END_SYMBOL
----------------

Existing behaviour, not accepted. The following program leads to an error and
should not lead to an error: `get get +. 123 456`. The program prints `579` and
then issues the error about a missing END or period.

Summary
=======

This updated version of Joy is slowly drifting away from the legacy version,
this version: JOY  -  compiled at 16:57:51 on Mar 17 2003.

- Some builtins have been added since that date: `condnestrec`, `fget`,
  `getch`, and `sametype`.

- The paper `j09imp.html` states: "When input reverts to an earlier file, the
  earlier line numbering is resumed." That functionality was added.

- The program is 64 bits by default. There is the option of compiling with
  BIT_32, but that is no longer tested.

- Local symbols are fully supported: they can call each other and do not
  show up in the symbol table. This should be considered as implementation
  dependent.

- The function `Compare` is used to enforce the same type of equality in all
  comparison operators.
