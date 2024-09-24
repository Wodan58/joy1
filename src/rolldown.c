/*
    module  : rolldown.c
    version : 1.5
    date    : 09/17/24
*/
#ifndef ROLLDOWN_C
#define ROLLDOWN_C

/**
Q0  OK  1240  rolldown  :  X Y Z  ->  Y Z X
Moves Y and Z down, moves X up.
*/
void rolldown_(pEnv env)
{
    Node *first, *second, *third;

    THREEPARAMS("rolldown");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck);
    third = env->stck;
    GUNARY(second);
    GNULLARY(first);
    GNULLARY(third);
}
#endif
