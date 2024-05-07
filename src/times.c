/*
    module  : times.c
    version : 1.6
    date    : 04/29/24
*/
#ifndef TIMES_C
#define TIMES_C

/**
OK 2800  times  :  N [P]  ->  ...
N times executes P.
*/
void times_(pEnv env)
{
    int i, n;
    Node *program;

    TWOPARAMS("times");
    ONEQUOTE("times");
    program = env->stck->u.lis;
    POP(env->stck);
    POSITIVEINTEGER("times");
    n = env->stck->u.num;
    POP(env->stck);
    for (i = 0; i < n; i++)
        exeterm(env, program);
}
#endif
