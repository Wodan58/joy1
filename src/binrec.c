/*
    module  : binrec.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef BINREC_C
#define BINREC_C

/**
2750  binrec  :  [P] [T] [R1] [R2]  ->  ...
Executes P. If that yields true, executes T.
Else uses R1 to produce two intermediates, recurses on both,
then executes R2 to combine their results.
*/
PRIVATE void binrecaux(
    pEnv env, Node *first, Node *second, Node *third, Node *fourth)
{
    Node *save;
    int result;

    save = env->stck;
    exeterm(env, first);
    result = env->stck->u.num;
    env->stck = save;
    if (result)
        exeterm(env, second);
    else {
        exeterm(env, third); /* split */
        save = env->stck;
        POP(env->stck);
        binrecaux(env, first, second, third, fourth); /* first */
        GNULLARY(save->op, save->u);
        binrecaux(env, first, second, third, fourth); /* second */
        exeterm(env, fourth); /* combine */
    }
}

PRIVATE void binrec_(pEnv env)
{
    Node *first, *second, *third, *fourth;

    FOURPARAMS("binrec");
    FOURQUOTES("binrec");
    fourth = env->stck->u.lis;
    POP(env->stck);
    third = env->stck->u.lis;
    POP(env->stck);
    second = env->stck->u.lis;
    POP(env->stck);
    first = env->stck->u.lis;
    POP(env->stck);
    binrecaux(env, first, second, third, fourth);
}
#endif
