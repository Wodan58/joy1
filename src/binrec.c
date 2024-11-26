/*
    module  : binrec.c
    version : 1.9
    date    : 11/11/24
*/
#ifndef BINREC_C
#define BINREC_C

#include "boolean.h"

/**
Q4  OK  2730  binrec  :  [P] [T] [R1] [R2]  ->  ...
Executes P. If that yields true, executes T.
Else uses R1 to produce two intermediates, recurses on both,
then executes R2 to combine their results.
*/
static void binrecaux(pEnv env, Node *prog[])
{
    Node *save;
    int result;

    save = env->stck;
    exeterm(env, prog[0]);
    result = get_boolean(env, env->stck);
    env->stck = save;
    if (result)
	exeterm(env, prog[1]);
    else {
	exeterm(env, prog[2]);	/* split */
	save = env->stck;
	POP(env->stck);
	binrecaux(env, prog);	/* first */
	GNULLARY(save);
	binrecaux(env, prog);	/* second */
	exeterm(env, prog[3]);	/* combine */
    }
}

void binrec_(pEnv env)
{
    Node *prog[4];

    FOURPARAMS("binrec");
    FOURQUOTES("binrec");
    prog[3] = env->stck->u.lis;
    POP(env->stck);
    prog[2] = env->stck->u.lis;
    POP(env->stck);
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    binrecaux(env, prog);
}
#endif
