 ![](Wynn.PNG)
==============

Changes
=======

This updated version of Joy is slightly different from the legacy version,
this version: JOY  -  compiled at 16:57:51 on Mar 17 2003.

- Some bugs have been removed. No guarantee can be given that none are left:
  the code base is simply too large to allow such a guarantee.

- Better datastructures are used. This allows growth of symbol table,
  tokenlist, symbols, and include search directories.

- A build system has been added, allowing easy addition or removal of new
  builtins.

- Some builtins have been added, positioned after `quit`. If they are not used
  and CONST is also not used, the language is the same as it was in 2003.

- CONST and its synonym INLINE have been added. They allow compile time
  evaluation.

- The user manual has been updated with annotations, documenting some of the
  above.

Stating that this repository differs slightly from the legacy version is a bit
of an understatement. All source files have been modified. The statement is
true w.r.t. the language. The aim of this repository is to not change the
language.

Roadmap
=======

If the programming language is changed, it might use one of the following:

Extensions
----------

There are new builtings `assign` and `unassign` and some others in between.
The first one is mentioned in chapter 18 of the book
[Symbolic Processing in Pascal](https://github.com/nickelsworth/sympas/blob/master/text/18-minijoy.org),
and Joy with assignment is called `extended Joy`. These extensions need not
be used and in that case the language is still the same as it was in 2003.

Compiler
--------

There is some conditional compilation, activated by -DBYTECODE or -DCOMPILER
that adds the option to compile Joy source code to bytecode or C.

At this moment there are no repositories with Joy source code that could
benefit from such compilation and because of that, these options are not
activated.

When activated, they will only be available for joy1 and Moy, not Joy, due to
the incompatible Node type of Joy.

Bignum
------

There is also a bignum type, but no big numbers. There is no application yet
that would need them.
