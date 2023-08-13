/*
    module  : rotate.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef ROTATE_C
#define ROTATE_C

/**
OK 1250  rotate  :  X Y Z  ->  Z Y X
Interchanges X and Z.
*/
PRIVATE void rotate_(pEnv env)
{
    Node *first, *second, *third;

    THREEPARAMS("rotate");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck);
    third = env->stck;
    GUNARY(first->op, first->u);
    GNULLARY(second->op, second->u);
    GNULLARY(third->op, third->u);
}
#endif
