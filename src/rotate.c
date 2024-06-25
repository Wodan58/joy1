/*
    module  : rotate.c
    version : 1.4
    date    : 06/21/24
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
    GUNARY(first);
    GNULLARY(second);
    GNULLARY(third);
}
#endif
