/*
    module  : rolldown.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef ROLLDOWN_C
#define ROLLDOWN_C

/**
1240  rolldown  :  X Y Z  ->  Y Z X
Moves Y and Z down, moves X up.
*/
PRIVATE void rolldown_(pEnv env)
{
    Node *first, *second, *third;

    THREEPARAMS("rolldown");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck);
    third = env->stck;
    GUNARY(second->op, second->u);
    GNULLARY(first->op, first->u);
    GNULLARY(third->op, third->u);
}
#endif
