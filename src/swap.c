/*
    module  : swap.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef SWAP_C
#define SWAP_C

/**
1220  swap  :  X Y  ->  Y X
Interchanges X and Y on top of the stack.
*/
PRIVATE void swap_(pEnv env)
{
    Node *first, *second;

    TWOPARAMS("swap");
    first = env->stck;
    POP(env->stck);
    second = env->stck;
    GUNARY(first->op, first->u);
    GNULLARY(second->op, second->u);
}
#endif
