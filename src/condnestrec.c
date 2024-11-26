/*
    module  : condnestrec.c
    version : 1.9
    date    : 11/11/24
*/
#ifndef CONDNESTREC_C
#define CONDNESTREC_C

#include "boolean.h"

/**
Q1  OK  2750  condnestrec  :  [ [C1] [C2] .. [D] ]  ->  ...
A generalisation of condlinrec.
Each [Ci] is of the form [[B] [R1] [R2] .. [Rn]] and [D] is of the form
[[R1] [R2] .. [Rn]]. Tries each B, or if all fail, takes the default [D].
For the case taken, executes each [Ri] but recurses between any two
consecutive [Ri] (n > 3 would be exceptional.)
*/
static void condnestrecaux(pEnv env, Node *list)
{
    int result = 0;
    Node *my_dump, *save;

    my_dump = list;
    save = env->stck;
    while (!result && my_dump && my_dump->next) {
	env->stck = save;
	exeterm(env, my_dump->u.lis->u.lis);
	result = get_boolean(env, env->stck);
	if (!result)
	    my_dump = my_dump->next;
    }
    env->stck = save;
    my_dump = result ? my_dump->u.lis->next : my_dump->u.lis;
    exeterm(env, my_dump->u.lis);
    my_dump = my_dump->next;
    while (my_dump) {
	condnestrecaux(env, list);
	exeterm(env, my_dump->u.lis);
	my_dump = my_dump->next;
    }
}

void condnestrec_(pEnv env)
{
    Node *list;

    ONEPARAM("condnestrec");
    LIST("condnestrec");
    CHECKEMPTYLIST(env->stck->u.lis, "condnestrec");
    list = env->stck->u.lis;
    POP(env->stck);
    condnestrecaux(env, list);
}
#endif
