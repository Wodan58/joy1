/*
    module  : app1.c
    version : 1.7
    date    : 10/11/24
*/
#ifndef APP1_C
#define APP1_C

/**
Q1  OK  2440  app1  :  X [P]  ->  R
Obsolescent.  Executes P, pushes result R on stack.
*/
void app1_(pEnv env)
{
    TWOPARAMS("app1");
    ONEQUOTE("app1");
    i_(env);
}
#endif
