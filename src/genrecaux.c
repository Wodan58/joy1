/*
    module  : genrecaux.c
    version : 1.10
    date    : 01/14/25
*/
#ifndef GENRECAUX_C
#define GENRECAUX_C

#include "boolean.h"
#include "cons.c"

/**
Q1  OK  3240  #genrecaux  :  [[B] [T] [R1] R2]  ->  ...
Executes B, if that yields true, executes T.
Else executes R1 and then [[[B] [T] [R1] R2] genrec] R2.
*/
void genrecaux_(pEnv env)
{
    int result;
    Node *prog, *save;

    prog = env->stck->u.lis;
    POP(env->stck);
    save = env->stck;
    exeterm(env, prog->u.lis);			/* [B] */
    CHECKSTACK("genrecaux");
    result = get_boolean(env, env->stck);
    env->stck = save;
    if (result)
	exeterm(env, prog->next->u.lis);	/* [T] */
    else {
	exeterm(env, prog->next->next->u.lis);	/* [R1] */
	NULLARY(LIST_NEWNODE, prog);
	save = ANON_FUNCT_NEWNODE(genrecaux_, 0);
	NULLARY(LIST_NEWNODE, save);
	cons_(env);
	exeterm(env, prog->next->next->next);	/* [R2] */
    }
}
#endif
