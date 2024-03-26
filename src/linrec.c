/*
    module  : linrec.c
    version : 1.5
    date    : 03/21/24
*/
#ifndef LINREC_C
#define LINREC_C

/**
OK 2710  linrec  :  [P] [T] [R1] [R2]  ->  ...
Executes P. If that yields true, executes T.
Else executes R1, recurses, executes R2.
*/
void linrecaux(pEnv env, Node *first, Node *second, Node *third, Node *fourth)
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

void linrec_(pEnv env)
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
