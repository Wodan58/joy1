/*
    module  : while.c
    version : 1.8
    date    : 10/11/24
*/
#ifndef WHILE_C
#define WHILE_C

/**
Q2  OK  2700  while  :  [B] [D]  ->  ...
While executing B yields true executes D.
*/
void while_(pEnv env)
{
    int result;
    Node *prog[2], *save;

    TWOPARAMS("while");
    TWOQUOTES("while");
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    while (1) {
	save = env->stck;
	exeterm(env, prog[0]);
	CHECKSTACK("while");
	result = env->stck->u.num;
	env->stck = save;
	if (!result)
	    break;
	exeterm(env, prog[1]);
    }
}
#endif
