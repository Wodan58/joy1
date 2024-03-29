/*
    module  : ifte.c
    version : 1.4
    date    : 03/21/24
*/
#ifndef IFTE_C
#define IFTE_C

/**
OK 2600  ifte  :  [B] [T] [F]  ->  ...
Executes B. If that yields true, then executes T else executes F.
*/
void ifte_(pEnv env)
{
    int num;
    Node *second, *first, *test, *save;

    THREEPARAMS("ifte");
    THREEQUOTES("ifte");
    second = env->stck->u.lis;
    POP(env->stck);
    first = env->stck->u.lis;
    POP(env->stck);
    test = env->stck->u.lis;
    POP(env->stck);
    save = env->stck;
    exeterm(env, test);
    num = env->stck->u.num;
    env->stck = save;
    exeterm(env, num ? first : second);
}
#endif
