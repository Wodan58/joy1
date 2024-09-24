/*
    module  : i.c
    version : 1.5
    date    : 09/17/24
*/
#ifndef I_C
#define I_C

/**
Q1  OK  2410  i  :  [P]  ->  ...
Executes P. So, [P] i  ==  P.
*/
void i_(pEnv env)
{
    Node *save;

    ONEPARAM("i");
    ONEQUOTE("i");
    save = env->stck;
    POP(env->stck);
    exeterm(env, save->u.lis);
}
#endif
