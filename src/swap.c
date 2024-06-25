/*
    module  : swap.c
    version : 1.4
    date    : 06/21/24
*/
#ifndef SWAP_C
#define SWAP_C

/**
OK 1220  swap  :  X Y  ->  Y X
Interchanges X and Y on top of the stack.
*/
void swap_(pEnv env)
{
    Node *first, *second;

    TWOPARAMS("swap");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    GUNARY(first);
    GNULLARY(second);
}
#endif
