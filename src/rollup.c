/*
    module  : rollup.c
    version : 1.5
    date    : 09/17/24
*/
#ifndef ROLLUP_C
#define ROLLUP_C

/**
Q0  OK  1230  rollup  :  X Y Z  ->  Z X Y
Moves X and Y up, moves Z down.
*/
void rollup_(pEnv env)
{
    Node *first, *second, *third;

    THREEPARAMS("rollup");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck);
    third = env->stck;
    GUNARY(first);
    GNULLARY(third);
    GNULLARY(second);
}
#endif
