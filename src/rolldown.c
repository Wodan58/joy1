/*
    module  : rolldown.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef ROLLDOWN_C
#define ROLLDOWN_C

/**
OK 1240  rolldown  :  X Y Z  ->  Y Z X
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
