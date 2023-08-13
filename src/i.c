/*
    module  : i.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef I_C
#define I_C

/**
OK 2430  i  :  [P]  ->  ...
Executes P. So, [P] i  ==  P.
*/
PRIVATE void i_(pEnv env)
{
    Node *save;

    ONEPARAM("i");
    ONEQUOTE("i");
    save = env->stck;
    POP(env->stck);
    exeterm(env, save->u.lis);
}
#endif
