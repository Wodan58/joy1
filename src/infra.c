/*
    module  : infra.c
    version : 1.3
    date    : 09/04/23
*/
#ifndef INFRA_C
#define INFRA_C

/**
OK 2810  infra  :  L1 [P]  ->  L2
Using list L1 as stack, executes P and returns a new list L2.
The first element of L1 is used as the top of stack,
and after execution of P the top of stack becomes the first element of L2.
*/
PRIVATE void infra_(pEnv env)
{
    Node *program, *save;

    TWOPARAMS("infra");
    ONEQUOTE("infra");
    LIST2("infra");
    program = env->stck->u.lis;
    save = env->stck->next->next;
    env->stck = env->stck->next->u.lis;
    exeterm(env, program);
    env->stck = LIST_NEWNODE(env->stck, save);
}
#endif
