/*
    module  : times.c
    version : 1.3
    date    : 08/13/23
*/
#ifndef TIMES_C
#define TIMES_C

/**
OK 2820  times  :  N [P]  ->  ...
N times executes P.
*/
PRIVATE void times_(pEnv env)
{
    int i, n;
    Node *program;

    TWOPARAMS("times");
    ONEQUOTE("times");
    program = env->stck->u.lis;
    POP(env->stck);
    INTEGER("times");
    n = env->stck->u.num;
    POP(env->stck);
    for (i = 0; i < n; i++)
        exeterm(env, program);
}
#endif
