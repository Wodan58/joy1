/*
    module  : dip.c
    version : 1.5
    date    : 06/21/24
*/
#ifndef DIP_C
#define DIP_C

/**
OK 2430  dip  :  X [P]  ->  ...  X
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
