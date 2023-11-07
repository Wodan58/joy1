 ![](Wynn.PNG)
==============

Introduction
============

Changes that were mentioned in globals.h are described here. A summary of the
changes is at the end of this page.

Recent changes
==============

The macro NEWNODE was added to globals.h, because of unspecified behaviour.

Accepted changes
================

Changes that have been accepted, because they corrected simple mistakes are
also mentioned here. They have been removed from globals.h.

CORRECT\_INTERN\_LOOKUP
-----------------------

The builtin `intern` allows interning of symbols with spaces or other
characters that do not adhere to the naming restrictions of identifiers. This
looks like a mistake, but it is not necessary to have this corrected. After
all, there exists a different way of accessing symbols: `"symbol" intern ==
[symbol] first`. In both cases, the symbol `symbol` will be placed on the
stack. There are 3 exceptions to the equality of `intern` and `[ .. ] first`:
`false`, `true`, and `maxint`. Thus: `"false" intern != [false] first`. The
word `intern` returns the function `false`, whereas the construct with `first`
returns the value `false`.

READ\_PRIVATE\_AHEAD, APPLY\_FORWARD\_SYMBOL
--------------------------------------------

This macro surrounds the code that enables local symbols and also public member
functions to call each other. There is a workaround: insert an empty forward
declaration.

SEARCH\_EXEC\_DIRECTORY
-----------------------

This macro was added in order to support out-of-source builds with Cmake. It
can be helpful during operating Joy as well: Joy loads `usrlib.joy` and
searches this library in the current directory. With this macro enabled Joy can
locate `usrlib.joy` also in another directory: the same directory as was used
earlier on.

USE\_ONLY\_STDIN
----------------

Joy gets input from stdin, even when taking input from a file, because that
file replaces stdin. That means that if there is a `get` inside the program
this `get` reads from the file that contains the Joy source code. And this is
exactly the kind of behaviour that is expected. So, `get get +. 123 456` can
occur in Joy source code and the input to `get` is present in the same source
code. The file descriptor that Joy uses to read input from is called `srcfile`.
The reason is that a file that is being read may include another file. Input
from the first file is suspended until the included file is finished. Reading
from the included file also uses `srcfile`. The old file descripter is stored
and restored to `srcfile` once the included file is finished.

NO\_COMPILER\_WARNINGS
----------------------

This, of course, is a no brainer. Compiler warnings are there to signal dubious
or obsolete constructs that need to be replaced by better ones.

NO\_BANG\_AS\_PERIOD
--------------------

The old source code had a `!` as an alternative for the period at the end of a
program. The reason was the processing of floating points. `1.` is a valid
floating point in C. That may be true, but Joy can have its own definition of a
floating point. In the Joy definition it is required to have digits on both
sides of the decimal separator. So, `1. ` with a non-digit character after the
decimal point is parsed as an integer, followed by a full stop, followed by
whatever comes thereafter. Joy is somewhat tied to C as the implementation
language but need not follow every quirck of that language.

PROTECT\_BUFFER\_OVERFLOW
-------------------------

The old source has a buffer of 25 characters where a floating point was
collected. First of all, it is not necessary to copy the characters, because
they are already available in the line buffer and more importantly, there was
no protection against buffer overflow. The problem of buffer overflow might
occur elsewhere in the source code.

USE\_SNPRINTF
-------------

`snprintf` is currently available in the most recent Windows compiler, and so
this define can be removed from the source code. If the source code is compiled
on older systems that do not have `snprintf`, then the programmer needs to grab
the source code of `snprintf` and add this to CMakeLists.txt. `snprintf` helps
to prevent buffer overflow.

DONT\_READ\_PAST\_EOF
---------------------

There are some .out files where this becomes visible through the character ÿ.
`joytut.out`, `lsptst.out`, and `grmtst.out` show the problem. The character
that is read in `getch` needs to be declared as `int`, not `char`.

CORRECT\_GARBAGE\_COLLECTOR
---------------------------

There are two issues with the original copying garbage collector. First, the
midpoint was calculated as `low + high/2` and that should be:
`low + (high - low) / 2`. The second problem was that there was no protection
against transgressing the midpoint. If that happens, all of memory is in use
and an out-of-memory should be reported.

READ\_NUMBER\_AND\_STOP
-----------------------

The document `j09imp.html` promises that octal and hexadecimal numbers are
supported, and indeed those are supported by `strtol`. The old code, however,
reads digits, +, -, ., E, and e. The reading should stop as soon as a number
has been read. More specifically, if a number is followed by a stop and then
a non-digit character, the non-digit character and stop should be pushed back
into the input stream. That is what this define tried to accomplish.

READ\_HEXADECIMAL\_NUMBER
-------------------------

As mentioned in the previous paragraph, hexadecimal digits A-F, or a-f were not
considered as part of a number. This define corrected that. In the end, a full
parser for a number was installed in the source code, just to be sure that a
number is read and nothing more than that. Pushing back into the input stream
is also unnecessary, because the input stream is line buffered and all
characters that are needed are available in the line buffer.

CORRECT\_STRING\_WRITE
----------------------

Ideally, the output from writefactor should be valid Joy source code. The old
version prints a string as `"%s"` and that fails if the string contains a `"`
or a newline. Also, if there are unreadable characters in the string, the
output will look strange. This define tries to repair that. The best attitude
towards this problem is to admit that the output of writefactor is not always
valid Joy source code.

BDW\_ALSO\_IN\_MAIN
-------------------

In `enterglobal` there was a call to `malloc`. When using the BDW garbage
collector it seems best to have all calls to `malloc` replaced with calls to
`GC_malloc`, although that is not mandatory. As it happens, the call to
GC\_strdup that was present in `main.c` is now used in `scan.c`.

NO\_JUMP\_FAIL
--------------

It looks unnecessary to have `setjmp` called twice. So this `fail` buffer,
that is also never used, was removed.

NO\_DUPLICATE\_CH
-----------------

There was a `ch` in `main`, as well as a global `ch`. The `ch` in `main` was
set to space and never used. Both lines of code can be removed.

USE\_NEW\_FUNCTION\_SYNTAX
--------------------------

There were a number of functions that still used the old K&R style declarations.
This define replaced them with the new ANSI syntax (new in 1989).

CORRECT\_OCTAL\_NUMBER
----------------------

As was mentioned a couple of paragraphs above, octal numbers were supported.
What that means is that as soon as an octal number has been spotted, it is
reported as such. More specifically: `08` is parsed as two numbers, `0` and `8`.
The reason that it cannot be an octal number is that `8` is not an octal digit.
Spaces between tokens are only mandatory when needed to separate two tokens.
Even more: `00` can be parsed as two numbers, both 0. That is not what was done
in Joy (it is that way in Moy). 

NO\_EXECUTE\_ERROR
------------------

The old version has a check at the end of a program that verifies that the
program ends with `.` or `END`. That check is not needed and also gave problems
in the construct: `get get +. 123 456`.

DONT\_ADD\_MODULE\_NAMES
------------------------

It does not seem necessary to add module names to the symbol table, as the old
version did. The define makes it possible to not have modules in the symbol
table.

CHECK\_SYMTABMAX
----------------

The old version did not check an overflow of the symbol table. This define adds
a message to that effect. This define is obsolete as the symbol table can now
grow when needed.

CHECK\_DISPLAYMAX
-----------------

The old version did not check overflow of the display of local symbols. This
define installs such a check. The displaymax is used for both modules and
local symbols.

HASHVALUE\_FUNCTION
-------------------

The old version had the code of the hash function in two locations. This define
replaces them by one location and has the two locations call this function.
As it is now, a new and better hash table implementation has been installed.

NO\_WASTE\_FP
-------------

The old version opens `usrlib.joy` to see if it is present and can be read.
The return value is not captured in a variable and the file is not closed.
That wastes a file pointer. File pointers are a limited resource that should
be respected.

RESPECT\_INPLINEMAX
-------------------

The old version stores characters that have been read in the line buffer,
without checking for overflow. This define corrects that. As it currently
stands, lines are read instead of characters, automatically respecting the
limit of the line buffer. As a consequence thereof, the input is now considered
to consist of lines, not characters. This also means that constants, such as
floating point constants cannot be larger than one line. If that becomes
necessary, it is possible to adjust INPLINEMAX.

CLOSE\_INCLUDE\_FILES
---------------------

The old version opens files with `fopen`, but does not close them after hitting
EOF. This wastes file pointers. File pointers are a limited resource and should
not be wasted. This define corrects that behaviour.

CORRECT\_GETENV
---------------

The function `getenv` can return a NULL pointer and that should be replaced by
"", an empty string. A NULL pointer is not a valid string in Joy. If it would
be accepted as such, all locations where a string is used, need protection
against NULL pointers. It is better to tackle the problem at the source and
replace the NULL with an empty string.

RESUME\_EARLIER\_LINENUM
------------------------

This behaviour was promised in `j09imp.html`, but not present in the old
version. The input stack needs to remember not only the file pointer, but also
the linenumber, such that this linenumber can be continued after an included
file was processed. As it happens, the filename is also remembered, and
currently also used.

CHECK\_QUOTE\_IN\_SPLIT
-----------------------

The function `split` checks that there are two parameters. The top most
parameter must be a quotation. This define adds that check.

CORRECT\_HELPDETAIL
-------------------

The function `helpdetail` can be improved. In case of the values `false`,
`true`, and `maxint`, it is desirable to print the description of the
functions, not the description of the values. Thus some translation from value
to function needs to take place. This define assists in that.

CHECK\_QUOTES\_IN\_PRIMREC
--------------------------

The function `primrec` checks that there are three parameters. The two top most
parameters must be quotations. This define adds that check.

CORRECT\_HEADERS
----------------

The headers were not correctly displayed in HTML. The headers are now embedded
in `<DT><BR><B>` and `</B><BR><BR>`.

HELP\_CONDNESTREC
-----------------

The help in `condnestrec` was not subdivided in lines. These lines are
necessary for a nice display.

CORRECT\_BINREC\_HELP
---------------------

The function `binrec` starts its explanation with `Executes P.`, but the stack
effect shows `[B]`. This was changed in `[P]`.

CORRECT\_FSEEK\_MANUAL
----------------------

The function `fseek` forgets to mention in the stack effect comment that it
pushes a boolean value indicating success or failure. This define corrects the
stack effect comment.

CORRECT\_HELP\_FREMOVE
----------------------

The function `fremove` mentions the push of a boolean indicating success or
failure, but fails to mention the `B`. This define adds the `B`.

CORRECT\_CLOCK\_SECONDS
-----------------------

The `clock` function was not entirely correct. It now mentions the number of
milliseconds.

CORRECT\_TIME\_LIST
-------------------

The `decode_time` function took the workday from the list, but the list
contains a corrected value for the workday. This correction must be reverted
in the `decode_time` function. This define makes sure that it happens.

CORRECT\_APP1\_DOC
------------------

The function `app1` is documented to take two parameters. That is correct.
What is not correct is that it says: 'Pushes result R on stack without X'.
The 'without X' is not correct. This defines corrects that.

CORRECT\_GENREC\_HELP
---------------------

The function `genrec` is documented with:
'and then [[B] [T] [R1] [R2] genrec] R2', but that is not what it does. The
text should be: 'and then [[[B] [T] [R1] R2] genrec] R2. This define corrects
that.

CORRECT\_TREEREC\_HELP
----------------------

The function `treerec` is documented with:
'Else executes [[O] [C] treerec] C.', but that should be:
'Else executes [[[O] C] treerec] C.'. This define corrects that.

CORRECT\_TREEGENREC\_HELP
-------------------------

The function `treegenrec` is documented with:
'and then [[O1] [O2] [C] treegenrec] C.', and that should be:
'and then [[[O1] [O2] C] treegenrec] C.'. This define corrects that.

CORRECT\_MODF\_CHECK
--------------------

The `modf` function checks that it has one parameter and that it is a float,
but uses `frexp` in the error message in case the conditions are not met.
This define corrects that.

CORRECT\_FORMATF\_MESSAGE
-------------------------

The `formatf` function had the same problem, in that it uses the text `format`
in four out of five error messages. This define corrects that.

TAILREC\_CHECK\_QUOTES
----------------------

The function `tailrec` checks that it receives three parameters. It fails to
check that it receives three quotations. This define adds that check.

TREEREC\_CHECK\_QUOTES
----------------------

The function `treerec` checks that it receives three parameters. It fails to
check that the two topmost parameters should be quotations. This define adds
that check.

TREEGENREC\_CHECK\_QUOTES
-------------------------

The function `treegenrec` checks that it receives four parameters. What it does
not do is check that the three topmost parameters are quotations. This define
adds that check.

CORRECT\_FIVE\_PARAMS
---------------------

This macro tests whether five parameters are present and if not, issues the
error "four parameters" instead of "five parameters". This define corrects that.

CORRECT\_INTERN\_STRCPY
-----------------------

The intern functions uses `strcpy` from the string parameter to the `id` buffer
without checking the length of the string. This define truncates the string, if
it is longer than ALEN, the size of the `id` buffer, and makes sure that the
last character is the 0 terminator of a string.

CORRECT\_NEG\_INTEGER
---------------------

The `neg` function creates a floating point from an integer when this function
is called. The type of the data should remain intact. That is what this define
accomplishes.

CORRECT\_STRFTIME\_BUF
----------------------

The strftime allocates a buffer that is way too small. Try
`time localtime "%c" strftime.`. This define allocates a larger buffer.

CORRECT\_FGETS
--------------

There is a bug in `fgets` that becomes visible after two reallocations. This
is caused by a wrong update of the length variable. This define corrects that
bug.

CORRECT\_TAKE\_STRING
---------------------

A string is copied without `strcpy` and without ending the string with a null
byte. This define adds the null byte.

CORRECT\_STRING\_CONCAT
-----------------------

There was nothing wrong with the old code; this define causes the functions
`strcpy` and `strcat` to be used. The aim was to remove bugs and not use the
opportunity to embellish the source code. This change is in the latter category.

CORRECT\_NOT\_FOR\_LIST
-----------------------

The old code looks at the `num` field to decide whether a LIST_ is null or not.
That is not ok: the code should look at the `lis` field. These fields may not
have the same size. This define corrects that.

CORRECT\_SIZE\_CHECK
--------------------

The old code has BADDATA as size check. This should be BADAGGREGATE. This
define corrects that.

SECURE\_PUTCHARS
----------------

The old code uses the string parameter directly in `printf` but that is not
secure. When the string parameter contains a `%` it allows access to memory
that is not foreseen and may have consequences. This define prints the string
parameter with `%s`.

CORRECT\_PRIMREC
----------------

The old source code that does not use the BDW garbage collector needs every
allocation to be attached to one of the roots for garbage collection. This
define make sure that `primrec` is compliant in this respect.

CORRECT\_TREEGENREC\_AUX
------------------------

The old source code that does not use the BDW garbage collector needs every
allocation to be attached to one of the roots for garbage collection. This
define make sure that `treegenrec_aux` is compliant in this respect.

CORRECT\_TREEREC\_AUX
---------------------

The old source code that does not use the BDW garbage collector needs every
allocation to be attached to one of the roots for garbage collection. This
define make sure that `treerec_aux` is compliant in this respect.

CHECK\_EMPTY\_STACK
-------------------

There is the problem of the program `[1 2 3] [pop] map.` The builtin `map`
needs an entry on the stack that can be used in the newly created list. The
`pop` makes sure that such entry is not present. This causes a crash. The
define checks that the stack is not empty, triggering a runtime error instead
of a crash. The same problem also occurs in many other builtins and may not
have been corrected everywhere.

CORRECT\_NULL\_CASES
--------------------

The legacy code uses the `num` field as default, instead of using the
appropriate field for each of the datatypes. This define corrects that omission.

MAKE\_CONTS\_OBSOLETE
---------------------

The BDW version does not use `conts` and that is why this define outcomments
`conts` in the BDW version. In the NOBDW version `conts` is essential because
it is a root for garbage collection. The `conts` builtin itself is not very
useful.

FGET\_FROM\_FILE
----------------

There are a number of builtins that read from stdin; the same number of
builtins should be available for reading from a different file descriptor.
`fget` fills part of that gap.

GETCH\_AS\_BUILTIN
------------------

There are a number of file operations that send data to output, for example
`putch`. So, why not have the same number of operations on input? `getch` fills
that gap. It could be defined as: `stdin fgetch popd`.

SAMETYPE\_BUILTIN
-----------------

The predicate `sametype` was lost during the development of Joy and is a useful
addition. It allows some datatype specific predicates to be replaced by
`sametype`, if needed.

CORRECT\_TYPE\_COMPARE, CORRECT\_CASE\_COMPARE
----------------------------------------------

It would be good to have only one definition of equality. `Compare` compares
each type with every other type and is a robust way to enforce the same kind of
equality in `compare,equal,case,in,has,=,<` and other comparison operators. But
`in` has a problem: it breaks `grmtst.joy`. This problem was finally mitigated
by using a nickname: plus and ast instead of + and \*.

ONLY\_LOGICAL\_NOT
------------------

It looks ok to have `not` only available for BOOLEAN\_ and SET\_. After all,
`not` inverts a value and while it is possible to change every other value into
0, inverting 0 is only possible if there is only 1 value to invert to. Also, if
`not` is available for other datatypes, it overlaps functionality with `null`.

CORRECT\_FREAD\_PARAM
---------------------

The builtin `fread` throws away the file descriptor it reads from and that is
different from the documentation. This looks like a mistake, but correcting it
might break existing programs.

REST\_OF\_UNIX\_ESCAPES
-----------------------

Character escape sequences are easier to remember when this is enabled: all
ASCII values between 8 and 13 inclusive can then be escaped in a symbolic way.
Not that there is an urgent need for this addition, because escaping can also
be done numerically: '\n is equal to '\010.

CORRECT\_ALEN
-------------

The longest identifier in the sources is 21 characters long. Including the
terminating 0 byte, a buffer of 22 characters is needed. On the other hand, if
identifiers are going to be stored in a database, the shorter definition of 20
might be preferable. And no matter what value is given to ALEN, there will
always be identifiers that are longer.

NO\_HELP\_LOCAL\_SYMBOLS
------------------------

Symbols from the PRIVATE section of a module or symbols that are defined
between HIDE and IN end up in the symbol table. This change prevents them from
showing up in the symbol table. This also means that the output of `alljoy.joy`
will be different when this change is enabled. Not having local symbols shown
seems good to have, because they cannot be used outside their respective PUBLIC
sections. This is now accepted and not present in the source anymore.

USE\_UNKNOWN\_SYMBOLS
---------------------

This definition makes it possible that local symbols can call each other. The
only way to make it a 100% solution is to read the PRIVATE sections twice:
during the first read defined symbols are registered in the symbol table and
during the second read normal processing is done. The benefit of this
approach is that symbols, because they can be made local, do not show up in
the symbol table. There is an other approach that requires no change: simply
use forward declarations in the PRIVATE sections. For some implementations of
the symbol table this might be easier to implement.

CORRECT\_INHAS\_COMPARE
-----------------------

This definition would break `grmtst.joy` and that is not allowed. All
comparison operators now use the Compare function and a redefinition of `in`
in `grmtst.joy` is no longer necessary.

CORRECT\_SET\_MEMBER
--------------------

A set can only contain small numbers or characters. This define inserts a check
during the read of a set with the effect that a set is composed properly and
numbers that are too large are flagged as out of bounds.

AUTOMATIC\_NUM\_TO\_DBL
-----------------------

The document `j09imp.html` warns that numeric constants should not be larger
than what a machine word can contain. This define allows that restriction to be
forgotten. Literals that are too large for an integer are automatically
converted to double. With loss of precision and without warning.

Existing changes
================

Some existing functionality has been guarded with defines.

USE\_SHELL\_ESCAPE
------------------

This is existing behaviour of Joy, that is surrounded by this definition. There
might be a security issue here.

ENABLE\_TRACEGC
---------------

Likewise, existing behaviour. It reports on memory allocations in the NOBDW
version.

RUNTIME\_CHECKS, NCHECK
-----------------------

Likewise, existing behaviour. It is not recommended to turn this off, because
it takes very little time to check parameters and it is a good thing to have,
catching bugs when running a program. The new name is now NCHECK with reverse
meaning. NCHECK looks like NDEBUG, but NDEBUG is turned on automatically by
Cmake in the Release version; turning on or off should be a deliberate decision.

Available defines
=================

The following defines are still available, to be used when compiling Joy.

ARRAY\_BOUND\_CHECKING
----------------------

This is currently not used. The problem with `exeterm` is that the default case
is not an error, but a function pointer. That might make this define useful.

COPYRIGHT
---------

This define should be activated in the makefile, because it allows the version
to be displayed and more importantly it displays whether the version is BDW or
NOBDW and whether the binary was compiled for Release or Debug.

DUMP\_TOKENS, DEBUG\_TOKENS
---------------------------

This define allows debugging of the token read ahead. And this read ahead needs
debugging. There is no proof that there is an error but the token list showed
that there were problems.

JVERSION
--------

The version needs to be given on the command line when compiling and is built
from a number of strings. The version is only visible when COPYRIGHT is also
defined.

NOBDW
-----

There are two versions, BDW and NOBDW. The distinction is made with this define
and this define should be given on the command line.

REMEMBER\_FILENAME
------------------

This define allows the filename to be remembered in the stack of input files.
The filename is now used in error messages.

STATS
-----

For statistical purposes, some counts can be remembered. These counts can be
used in the comparison of various algorithms. Timing the binary is also
possible and requires no change in the source code.

SYMBOLS
-------

This define allows the symbol table to be dumped at the end of execution. The
symbol table contains all builtins and all defined symbols. This define can be
used to verify that everything has been arrived correctly in the symbol table.

TRACING
-------

Tracing can be used to track the execution of a program. This looks very nice
but is only useful for small programs.

\_MSC\_VER, WIN32
-----------------

Some headers files are different under the Microsoft compiler compared with
other compilers. What this means is that there are constructs used that are not
standard C and such constructs should be avoided.

Unaccepted changes
==================

The changes listed here may have had their use at some point in time, but are
now considered not needed.

SIGNAL\_WRITE\_PIPE
-------------------

This oddly named define guarded code that sets the stdout stream to unbuffered.
The problem that it solved is that sometimes the stderr messages come inbetween
the normal stdout output. Setting stdout to unbuffered solves the problem in a
radical way. It has the disadvantage that normal output is slowed down. The
better solution, that is currently implemented, is to precede every output to
stderr by a flush of stdout.

TRACK\_USED\_SYMBOLS
--------------------

This definition uses the program to generate a list of symbols that are used in
the Joy source files; it could be used to detect what the largest length of a
symbol is.

NOT\_ALSO\_FOR\_FLOAT, NOT\_ALSO\_FOR\_FILE
-------------------------------------------

The builtin `not` was generic, but not defined for all datatypes. In hindsight,
it looks better to restrict the use of `not` to logicals and that makes these
definitions obsolete.

CHECK\_END\_SYMBOL
------------------

Existing behaviour, not accepted. The following program leads to an error and
should not lead to an error: `get get +. 123 456`. The program prints `579` and
then issues the error about a missing END or period.

CLEAR\_STACK\_ON\_ERROR
-----------------------

After an error or after `abort` has been called, the rest of the program is
skipped. The question is what to do with the stack. The stack should remain
intact and that is why this definition is not accepted.

SYMMETRIC\_PLUS\_MINUS
----------------------

There is asymmetry in plus and minus: adding or subtracting an integer is
allowed when the first operand is a character; the character cannot appear as
the second operand. There is a use case where it would be more efficient to
allow such a construct: `2 "test" [+] map.`. This program needs to push the 2
only once, using it again and again. It so happens that the efficiency reason
does not apply to Moy and so this change is not accepted.

CORRECT\_FLOAT\_BUFFER
----------------------

This define was used to enlarge the buffer where floating points are collected
to the maximum possible value. Whatever the value, given the nature of formatf,
no buffer is large enough. The proper way is to use `snprintf`. If that
function is not available, it should be created.

GET\_FROM\_STDIN
----------------

The builtin `get` reads a factor from the same source that is used as input for
the Joy virtual processor a.k.a. `exeterm`. That is how it should be. The
define redirected `get` to stdin, but that is misguided.

ORIGINAL\_JOY
-------------

There was the option to undo all changes by allowing this define active. The
number of defines became too large to handle and so have been removed. If it is
really necessary to compare the behaviour of the current version with the old
version, then the old version should be compiled separately.

REMOVE\_UNUSED\_ERRORCOUNT
--------------------------

The errorcount is not used and still present in `scan.c`. It is now surrounded
by `#if 0` ... `#endif`. If errorcount is larger than 0, then the source code
cannot be executed, because it will fail. The number of errors is not important.

BIT\_32
-------

There was the possibility to compile for 32 bit computers. As this is not
tested anymore, this option has been removed.

COSMO
-----

Cosmopolitan is a library that allows the creation of binaries that can be
executed under Windows, Linux, Unix. The library can be used without changes to
the source code and that is why this define has been removed.

SINGLE
------

This define allowed the source to be compiled for either Joy or joy1. Instead
of that, the sources in the src-directory have been split.

\_\_linux\_\_
-------------

This define allowed the use of Linux-specific code to be executed. Such an
approach is misguided: the source code should be portable among compilers and
operating systems and thus should adhere to the current C standard. If that
prevents certain extensions, so be it.

TYPED\_NODES
------------

This defines was used to register extra type information in each of the nodes
that are used in memory. The reason why this was needed is lost in time.

USE\_TIME\_REC
--------------

This define was used to surround the use of gmtime\_r and localtime\_r. As it
is now, they are substitute functions specific to WIN32.

Summary
=======

This updated version of Joy is slowly drifting away from the legacy version,
this version: JOY  -  compiled at 16:57:51 on Mar 17 2003.

- Some builtins have been added since that date: `condnestrec`, `casting`,
  `fget`, `filetime`, `getch`, `over`, `pick`, `round`, `sametype`, and
  ` typeof`.

- The paper `j09imp.html` states: "When input reverts to an earlier file, the
  earlier line numbering is resumed." That functionality was added.

- The program is 64 bits by default. There was the option to compile with
  32 bits, but this has been removed.

- Local symbols are fully supported: they can call each other and do not
  show up in the symbol table.

- The function `Compare` is used to enforce the same type of equality in all
  comparison operators.
