/*
    module  : dip.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef DIP_C
#define DIP_C

/**
OK 2450  dip  :  X [P]  ->  ...  X
Saves X, executes P, pushes X back.
*/
PRIVATE void dip_(pEnv env)
{
    Node *save;

    TWOPARAMS("dip");
    ONEQUOTE("dip");
    save = env->stck;
    POP(env->stck);
    POP(env->stck);
    exeterm(env, save->u.lis);
    GNULLARY(save->next->op, save->next->u);
}
#endif
