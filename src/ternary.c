/*
    module  : ternary.c
    version : 1.5
    date    : 10/11/24
*/
#ifndef TERNARY_C
#define TERNARY_C

/**
Q1  OK  2570  ternary  :  X Y Z [P]  ->  R
Executes P, which leaves R on top of the stack.
No matter how many parameters this consumes,
exactly three are removed from the stack.
*/
N_ARY(ternary_, "ternary", FOURPARAMS, nextnode3(env->stck))

#endif
