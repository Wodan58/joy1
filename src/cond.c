/*
    module  : cond.c
    version : 1.9
    date    : 11/11/24
*/
#ifndef COND_C
#define COND_C

#include "boolean.h"

/**
Q1  OK  2690  cond  :  [..[[Bi] Ti]..[D]]  ->  ...
Tries each Bi. If that yields true, then executes Ti and exits.
If no Bi yields true, executes default D.
*/
void cond_(pEnv env)
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
	result = get_boolean(env, env->stck);
	if (!result)
	    my_dump = my_dump->next;
    }
    env->stck = save;
    if (result)
	exeterm(env, my_dump->u.lis->next);
    else
	exeterm(env, my_dump->u.lis);	/* default */
}
#endif
