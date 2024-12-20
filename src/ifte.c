/*
    module  : ifte.c
    version : 1.8
    date    : 11/11/24
*/
#ifndef IFTE_C
#define IFTE_C

#include "boolean.h"

/**
Q3  OK  2600  ifte  :  [B] [T] [F]  ->  ...
Executes B. If that yields true, then executes T else executes F.
*/
void ifte_(pEnv env)
{
    int result;
    Node *prog[3], *save;

    THREEPARAMS("ifte");
    THREEQUOTES("ifte");
    prog[2] = env->stck->u.lis;
    POP(env->stck);
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    save = env->stck;
    exeterm(env, prog[0]);
    result = get_boolean(env, env->stck);
    env->stck = save;
    exeterm(env, result ? prog[1] : prog[2]);
}
#endif
