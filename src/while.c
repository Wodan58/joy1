/*
    module  : while.c
    version : 1.3
    date    : 08/13/23
*/
#ifndef WHILE_C
#define WHILE_C

/**
OK 2720  while  :  [B] [D]  ->  ...
While executing B yields true executes D.
*/
PRIVATE void while_(pEnv env)
{
    int num;
    Node *body, *test, *save;

    TWOPARAMS("while");
    TWOQUOTES("while");
    body = env->stck->u.lis;
    POP(env->stck);
    test = env->stck->u.lis;
    POP(env->stck);
    for (;;) {
        save = env->stck;
        exeterm(env, test);
        CHECKSTACK("while");
        num = env->stck->u.num;
        env->stck = save;
        if (!num)
            return;
        exeterm(env, body);
    }
}
#endif
