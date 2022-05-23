/*
    module  : cond.c
    version : 1.2
    date    : 05/17/22
*/
#ifndef COND_C
#define COND_C

/**
2710  cond  :  [..[[Bi] Ti]..[D]]  ->  ...
Tries each Bi. If that yields true, then executes Ti and exits.
If no Bi yields true, executes default D.
*/
PRIVATE void cond_(pEnv env)
{
    int result = 0;
    Node *my_dump, *save;

    ONEPARAM("cond");
    LIST("cond");
    CHECKEMPTYLIST(env->stck->u.lis, "cond");
    /* must check for QUOTES in list */
    for (my_dump = env->stck->u.lis; my_dump->next; my_dump = my_dump->next)
        CHECKLIST(my_dump->u.lis->op, "cond");
    my_dump = env->stck->u.lis;
    save = env->stck->next;
    while (!result && my_dump && my_dump->next) {
        env->stck = save;
        exeterm(env, my_dump->u.lis->u.lis);
        result = env->stck->u.num;
        if (!result)
            my_dump = my_dump->next;
    }
    env->stck = save;
    if (result)
        exeterm(env, my_dump->u.lis->next);
    else
        exeterm(env, my_dump->u.lis); /* default */
}
#endif
