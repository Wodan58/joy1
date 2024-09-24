/*
    module  : rotate.c
    version : 1.5
    date    : 09/17/24
*/
#ifndef ROTATE_C
#define ROTATE_C

/**
Q0  OK  1250  rotate  :  X Y Z  ->  Z Y X
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
    GUNARY(first);
    GNULLARY(second);
    GNULLARY(third);
}
#endif
