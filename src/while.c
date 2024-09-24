/*
    module  : while.c
    version : 1.7
    date    : 09/17/24
*/
#ifndef WHILE_C
#define WHILE_C

/**
Q2  OK  2700  while  :  [B] [D]  ->  ...
While executing B yields true executes D.
*/
void while_(pEnv env)
{
    int num;
    Node *body, *test, *save;

    TWOPARAMS("while");
    TWOQUOTES("while");
    body = env->stck->u.lis;
    POP(env->stck);
    test = env->stck->u.lis;
    POP(env->stck);
    while (1) {
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
