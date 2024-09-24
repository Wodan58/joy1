/*
    module  : app1.c
    version : 1.6
    date    : 09/17/24
*/
#ifndef APP1_C
#define APP1_C

/**
Q1  OK  2440  app1  :  X [P]  ->  R
Obsolescent.  Executes P, pushes result R on stack.
*/
void app1_(pEnv env)
{
    Node *save;

    TWOPARAMS("app1");
    ONEQUOTE("app1");
    save = env->stck;
    POP(env->stck);
    exeterm(env, save->u.lis);
}
#endif
