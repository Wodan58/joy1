/*
    module  : app1.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef APP1_C
#define APP1_C

/**
2460  app1  :  X [P]  ->  R
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
