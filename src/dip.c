/*
    module  : dip.c
    version : 1.6
    date    : 09/17/24
*/
#ifndef DIP_C
#define DIP_C

/**
Q1  OK  2430  dip  :  X [P]  ->  ...  X
Saves X, executes P, pushes X back.
*/
void dip_(pEnv env)
{
    Node *save;

    TWOPARAMS("dip");
    ONEQUOTE("dip");
    save = env->stck;
    POP(env->stck);
    POP(env->stck);
    exeterm(env, save->u.lis);
    GNULLARY(save->next);
}
#endif
