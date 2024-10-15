/*
    module  : infra.c
    version : 1.6
    date    : 10/11/24
*/
#ifndef INFRA_C
#define INFRA_C

/**
Q1  OK  2810  infra  :  L1 [P]  ->  L2
Using list L1 as stack, executes P and returns a new list L2.
The first element of L1 is used as the top of stack,
and after execution of P the top of stack becomes the first element of L2.
*/
void infra_(pEnv env)
{
    Node *prog, *save;

    TWOPARAMS("infra");
    ONEQUOTE("infra");
    LIST2("infra");
    prog = env->stck->u.lis;
    save = env->stck->next->next;
    env->stck = env->stck->next->u.lis;
    exeterm(env, prog);
    env->stck = LIST_NEWNODE(env->stck, save);
}
#endif
