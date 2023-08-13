/*
    module  : app1.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef APP1_C
#define APP1_C

/**
OK 2460  app1  :  X [P]  ->  R
Executes P, pushes result R on stack.
*/
PRIVATE void app1_(pEnv env)
{
    Node *program;

    TWOPARAMS("app1");
    ONEQUOTE("app1");
    program = env->stck->u.lis;
    POP(env->stck);
    exeterm(env, program);
}
#endif
