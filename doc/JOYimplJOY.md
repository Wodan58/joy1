 ![](Wynn.PNG)
==============

Introduction
============

Changes that are mentioned in globals.h are described here. A summary of the
changes is at the end of this page.

Recent changes
==============

The macro NEWNODE was added to globals.h, because of unspecified behaviour.

Accepted changes
================

Changes that have been accepted, because they corrected simple mistakes are not
mentioned here. The ones that are mentioned are open for discussion.

FGET_FROM_FILE
--------------

There are a number of builtins that read from stdin; the same number of
builtins should be available for reading from a different file descriptor.
`fget` fills that gap.

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
`in` has a problem: it would break grmtst.joy and therefore `Compare` is not
used in `in`.

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
stack.

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
showing up in the symbol table. This means that the output of alljoy.joy will
be different when this change is enabled. Not having local symbols shown seems
good to have, because they cannot be used outside their respective PUBLIC
sections. This is now accepted and not present in the source anymore.

USE_UNKNOWN_SYMBOLS
-------------------

This definition makes it possible that local symbols can call each other.
The only way to make it a 100% solution is to read the PRIVATE sections twice:
during the first time defined symbols are registered in the symbol table and
during the second time normal processing is done. The benefit of this
definition is that symbols, because they can be made local, do not show up in
the symbol table.

CORRECT_INHAS_COMPARE
---------------------

This definition would break grmtst.joy and that is not allowed. A redefinition
in grmtst.joy is used; all comparison operators now use the Compare function.

Existing changes
================

Some existing functionality has been guarded with defines, because they are
open for discussion.

USE_SHELL_ESCAPE
----------------

This is existing behaviour of Joy, that is surrounded by this definition. There
might be a security issue here.

ENABLE_TRACEGC
--------------

Likewise, existing behaviour. It reports on memory allocations when GC_BDW is
not used.

RUNTIME_CHECKS
--------------

Likewise, existing behaviour. It is not recommended to turn this off, because
it takes very little time to check parameters and it is a good thing to have,
catching bugs when running a program.

Unaccepted changes
==================

The changes listed here may have had their use at some point in time, but are
now considered not needed.

TRACK_USED_SYMBOLS
------------------

This definition uses the program to generate a list of symbols that are used in
the Joy source files; it can be used to detect what the greatest length of a
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

This updated version of Joy is slowly drifting away from the legacy version.

- Some builtins have been added: `fget`, `getch`, and `sametype`.

- When compiling with BDW enabled, `dump`, `conts`, and `memoryindex` are no
  longer used.

- The paper j09imp.html states: "When input reverts to an earlier file, the
  earlier line numbering is resumed." That functionality was added in this
  version.

- There is the option of compiling with BIT_64; also the floating point size is
  kept the same as the integer size.

- Local symbols are now fully supported: they can call each other and do not
  show up in the symbol table.

- The function `Compare` is used to enforce the same type of equality in all
  comparison operators.
