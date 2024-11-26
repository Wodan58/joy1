/*
    module  : filter.c
    version : 1.13
    date    : 11/11/24
*/
#ifndef FILTER_C
#define FILTER_C

#include "boolean.h"

/**
Q1  OK  2830  filter  :  A [B]  ->  A1
Uses test B to filter aggregate A producing sametype aggregate A1.
*/
void filter_(pEnv env)
{
    Node *prog, *my_dump1 = 0, /* step */
		*my_dump2 = 0, /* head */
	 *save, *my_dump3 = 0; /* last */
    char *volatile ptr;
    int i = 0, result = 0;
    char *str, *resultstr;
    uint64_t set, resultset;

    TWOPARAMS("filter");
    ONEQUOTE("filter");
    prog = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case SET_:
	set = env->stck->u.set;
	for (resultset = i = 0; i < SETSIZE; i++)
	    if (set & ((int64_t)1 << i)) {
		env->stck = INTEGER_NEWNODE(i, save);
		exeterm(env, prog);
		CHECKSTACK("filter");
		result = get_boolean(env, env->stck);
		if (result)
		    resultset |= ((int64_t)1 << i);
	    }
	env->stck = SET_NEWNODE(resultset, save);
	break;
    case STRING_:
	ptr = env->stck->u.str;		/* remember this */
	resultstr = GC_malloc_atomic(strlen(ptr) + 1);
	for (str = ptr; *str; str++) {
	    env->stck = CHAR_NEWNODE(*str, save);
	    exeterm(env, prog);
	    CHECKSTACK("filter");
	    result = get_boolean(env, env->stck);
	    if (result)
		resultstr[i++] = *str;
	}
	resultstr[i] = 0;
	env->stck = STRING_NEWNODE(resultstr, save);
	break;
    case LIST_:
	for (my_dump1 = env->stck->u.lis; my_dump1; my_dump1 = my_dump1->next) {
	    env->stck = newnode2(env, my_dump1, save);
	    exeterm(env, prog);
	    CHECKSTACK("filter");
	    result = get_boolean(env, env->stck);
	    if (result) {		/* test */
		if (!my_dump2)		/* first */
		    my_dump3 = my_dump2 = newnode2(env, my_dump1, 0);
		else			/* further */
		    my_dump3 = my_dump3->next = newnode2(env, my_dump1, 0);
	    }
	}
	env->stck = LIST_NEWNODE(my_dump2, save);
	break;
    default:
	BADAGGREGATE("filter");
    }
}
#endif
