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

- CONST and its synonym INLINE have been added. They allow compile time
  evaluation.

- Some builtins have been added, positioned after quit. If they are not used
  and CONST is also not used, the language is the same as it was in 2003.

- The user manual has been updated with annotations, documenting some of the
  above.

Compiler
========

There is some conditional compilation, activated by -DBYTECODE that adds an
option to compile Joy source code.

At this moment there is no repository with Joy source code that could benefit
from such compilation and because of that, the -DBYTECODE is not activated.

If it is activated, it will only be available for joy1, not Joy, due to the
incompatible Node type of Joy.

Also, the compiler is not very good at this moment.
