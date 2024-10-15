/*
    module  : binary.c
    version : 1.5
    date    : 10/11/24
*/
#ifndef BINARY_C
#define BINARY_C

/**
Q1  OK  2560  binary  :  X Y [P]  ->  R
Executes P, which leaves R on top of the stack.
No matter how many parameters this consumes,
exactly two are removed from the stack.
*/
N_ARY(binary_, "binary", THREEPARAMS, nextnode2(env->stck))

#endif
