/*
    module  : linrec.c
    version : 1.2
    date    : 05/02/22
*/
#ifndef LINREC_C
#define LINREC_C

/**
2730  linrec  :  [P] [T] [R1] [R2]  ->  ...
Executes P. If that yields true, executes T.
Else executes R1, recurses, executes R2.
*/
PRIVATE void linrecaux(
    pEnv env, Node *first, Node *second, Node *third, Node *fourth)
{
    Node *save;
    int result;

    save = env->stck;
    exeterm(env, first);
    CHECKSTACK("linrec");
    result = env->stck->u.num;
    env->stck = save;
    if (result)
        exeterm(env, second);
    else {
        exeterm(env, third);
        linrecaux(env, first, second, third, fourth);
        exeterm(env, fourth);
    }
}

PRIVATE void linrec_(pEnv env)
{
    Node *first, *second, *third, *fourth;

    FOURPARAMS("linrec");
    FOURQUOTES("linrec");
    fourth = env->stck->u.lis;
    POP(env->stck);
    third = env->stck->u.lis;
    POP(env->stck);
    second = env->stck->u.lis;
    POP(env->stck);
    first = env->stck->u.lis;
    POP(env->stck);
    linrecaux(env, first, second, third, fourth);
}
#endif
