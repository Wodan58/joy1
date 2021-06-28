/*
    module  : times.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef TIMES_C
#define TIMES_C

/**
2820  times  :  N [P]  ->  ...
N times executes P.
*/
PRIVATE void times_(pEnv env)
{
    int i, n;
    Node *program;

    TWOPARAMS("times");
    ONEQUOTE("times");
    INTEGER2("times");
    program = env->stck->u.lis;
    POP(env->stck);
    n = env->stck->u.num;
    POP(env->stck);
    for (i = 0; i < n; i++)
        exeterm(env, program);
}
#endif
