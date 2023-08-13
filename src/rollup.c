/*
    module  : rollup.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef ROLLUP_C
#define ROLLUP_C

/**
OK 1230  rollup  :  X Y Z  ->  Z X Y
Moves X and Y up, moves Z down.
*/
PRIVATE void rollup_(pEnv env)
{
    Node *first, *second, *third;

    THREEPARAMS("rollup");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck);
    third = env->stck;
    GUNARY(first->op, first->u);
    GNULLARY(third->op, third->u);
    GNULLARY(second->op, second->u);
}
#endif
