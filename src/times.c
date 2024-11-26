/*
    module  : times.c
    version : 1.10
    date    : 11/12/24
*/
#ifndef TIMES_C
#define TIMES_C

/**
Q1  OK  2800  times  :  N [P]  ->  ...
N times executes P.
*/
void times_(pEnv env)
{
    Node *prog;
    int64_t i, n;

    TWOPARAMS("times");
    ONEQUOTE("times");
    prog = env->stck->u.lis;
    POP(env->stck);
    n = env->stck->u.num;
    POP(env->stck);
    for (i = 0; i < n; i++)
	exeterm(env, prog);
}
#endif
