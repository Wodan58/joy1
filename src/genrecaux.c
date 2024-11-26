/*
    module  : genrecaux.c
    version : 1.9
    date    : 11/11/24
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

    prog = env->stck;
    POP(env->stck);
    save = env->stck;
    exeterm(env, prog->u.lis->u.lis);			/*	[B]	*/
    CHECKSTACK("genrecaux");
    result = get_boolean(env, env->stck);
    env->stck = save;
    if (result)
	exeterm(env, prog->u.lis->next->u.lis);		/*	[T]	*/
    else {
	exeterm(env, prog->u.lis->next->next->u.lis);	/*	[R1]	*/
	NULLARY(LIST_NEWNODE, prog->u.lis);
	save = ANON_FUNCT_NEWNODE(genrecaux_, 0);
	NULLARY(LIST_NEWNODE, save);
	cons_(env);
	exeterm(env, prog->u.lis->next->next->next);	/*	[R2]	*/
    }
}
#endif
