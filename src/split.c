/*
    module  : split.c
    version : 1.11
    date    : 10/11/24
*/
#ifndef SPLIT_C
#define SPLIT_C

/**
Q1  OK  2840  split  :  A [B]  ->  A1 A2
Uses test B to split aggregate A into sametype aggregates A1 and A2.
*/
void split_(pEnv env)
{
    Node *prog, *my_dump1 = 0, /* step */
		*my_dump2 = 0, /* head */
	 *save, *my_dump3 = 0, /* last */
		*my_dump4 = 0,
		*my_dump5 = 0;
    int i;
    size_t leng;
    char *volatile ptr;
    int yesptr = 0, noptr = 0;
    uint64_t set, yes_set = 0, no_set = 0;
    char *str, *yesstring = 0, *nostring = 0;

    TWOPARAMS("split");
    ONEQUOTE("split");
    prog = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case SET_:
	set = env->stck->u.set;
	for (i = 0; i < SETSIZE; i++)
	    if (set & ((int64_t)1 << i)) {
		env->stck = INTEGER_NEWNODE(i, save);
		exeterm(env, prog);
		CHECKSTACK("split");
		if (env->stck->u.num)
		    yes_set |= ((int64_t)1 << i);
		else
		    no_set |= ((int64_t)1 << i);
	    }
	env->stck = SET_NEWNODE(yes_set, save);
	NULLARY(SET_NEWNODE, no_set);
	break;
    case STRING_:
	ptr = env->stck->u.str;		/* remember this */
	leng = strlen(ptr) + 1;
	yesstring = GC_malloc_atomic(leng);
	nostring = GC_malloc_atomic(leng);
	for (str = ptr; *str; str++) {
	    env->stck = CHAR_NEWNODE(*str, save);
	    exeterm(env, prog);
	    CHECKSTACK("split");
	    if (env->stck->u.num)
		yesstring[yesptr++] = *str;
	    else
		nostring[noptr++] = *str;
	}
	yesstring[yesptr] = 0;
	nostring[noptr] = 0;
	env->stck = STRING_NEWNODE(yesstring, save);
	NULLARY(STRING_NEWNODE, nostring);
	break;
    case LIST_:
	for (my_dump1 = env->stck->u.lis; my_dump1; my_dump1 = my_dump1->next) {
	    env->stck = newnode2(env, my_dump1, save);
	    exeterm(env, prog);
	    CHECKSTACK("split");
	    if (env->stck->u.num) {	/* pass */
		if (!my_dump2)		/* first */
		    my_dump3 = my_dump2 = newnode2(env, my_dump1, 0);
		else			/* further */
		    my_dump3 = my_dump3->next = newnode2(env, my_dump1, 0);
	    } else {			/* fail */
		if (!my_dump4)		/* first */
		    my_dump5 = my_dump4 = newnode2(env, my_dump1, 0);
		else			/* further */
		    my_dump5 = my_dump5->next = newnode2(env, my_dump1, 0);
	    }
	}
	env->stck = LIST_NEWNODE(my_dump2, save);
	NULLARY(LIST_NEWNODE, my_dump4);
	break;
    default:
	BADAGGREGATE("split");
    }
}
#endif
