/*
    module  : rotate.c
    version : 1.3
    date    : 03/21/24
*/
#ifndef ROTATE_C
#define ROTATE_C

/**
OK 1250  rotate  :  X Y Z  ->  Z Y X
Interchanges X and Z.
*/
void rotate_(pEnv env)
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
