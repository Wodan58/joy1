/*
    module  : tailrec.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef TAILREC_C
#define TAILREC_C

PRIVATE void tailrecaux(pEnv env, Node *first, Node *second, Node *third)
{
    Node *save;
    int result;

tailrec:
    save = env->stck;
    exeterm(env, first);
    result = env->stck->u.num;
    env->stck = save;
    if (result)
        exeterm(env, second);
    else {
        exeterm(env, third);
        goto tailrec;
    }
}

/**
2740  tailrec  :  [P] [T] [R1]  ->  ...
Executes P. If that yields true, executes T.
Else executes R1, recurses.
*/
PRIVATE void tailrec_(pEnv env)
{
    Node *first, *second, *third;

    THREEPARAMS("tailrec");
    THREEQUOTES("tailrec");
    third = env->stck->u.lis;
    POP(env->stck);
    second = env->stck->u.lis;
    POP(env->stck);
    first = env->stck->u.lis;
    POP(env->stck);
    tailrecaux(env, first, second, third);
}
#endif
