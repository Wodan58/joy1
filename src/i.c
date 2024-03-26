/*
    module  : i.c
    version : 1.4
    date    : 03/21/24
*/
#ifndef I_C
#define I_C

/**
OK 2410  i  :  [P]  ->  ...
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
