/*
    module  : branch.c
    version : 1.8
    date    : 11/11/24
*/
#ifndef BRANCH_C
#define BRANCH_C

#include "boolean.h"

/**
Q2  OK  2590  branch  :  B [T] [F]  ->  ...
If B is true, then executes T else executes F.
*/
void branch_(pEnv env)
{
    int result;
    Node *prog[2];

    THREEPARAMS("branch");
    TWOQUOTES("branch");
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    result = get_boolean(env, env->stck);
    POP(env->stck);
    exeterm(env, result ? prog[0] : prog[1]);
}
#endif
