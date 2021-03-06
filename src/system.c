/*
    module  : system.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef SYSTEM_C
#define SYSTEM_C

/**
3040  system  :  "command"  ->
Escapes to shell, executes string "command".
The string may cause execution of another program.
When that has finished, the process returns to Joy.
*/
USETOP(system_, "system", STRING, (void)system(nodevalue(env->stck).str))



#endif
