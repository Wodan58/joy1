/*
    module  : map.c
    version : 1.13
    date    : 10/11/24
*/
#ifndef MAP_C
#define MAP_C

/**
Q1  OK  2790  map  :  A [P]  ->  B
Executes P on each member of aggregate A,
collects results in sametype aggregate B.
*/
void map_(pEnv env)
{
    Node *prog, *my_dump1 = 0, /* step */
		*my_dump2 = 0, /* head */
	 *save, *my_dump3 = 0; /* last */
    int i = 0;
    char *volatile ptr;
    uint64_t set, resultset;
    char *str = 0, *resultstr;

    TWOPARAMS("map");
    ONEQUOTE("map");
    prog = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case LIST_:
	for (my_dump1 = env->stck->u.lis; my_dump1; my_dump1 = my_dump1->next) {
	    env->stck = newnode2(env, my_dump1, save);
	    exeterm(env, prog);
	    CHECKSTACK("map");
	    if (!my_dump2)	/* first */
		my_dump3 = my_dump2 = newnode2(env, env->stck, 0);
	    else		/* further */
		my_dump3 = my_dump3->next = newnode2(env, env->stck, 0);
	}
	env->stck = LIST_NEWNODE(my_dump2, save);
	break;
    case STRING_:
	str = ptr = env->stck->u.str;
	resultstr = GC_malloc_atomic(strlen(ptr) + 1);
	for (; *str; str++) {
	    env->stck = CHAR_NEWNODE(*str, save);
	    exeterm(env, prog);
	    CHECKSTACK("map");
	    resultstr[i++] = env->stck->u.num;
	}
	resultstr[i] = 0;
	env->stck = STRING_NEWNODE(resultstr, save);
	break;
    case SET_:
	set = env->stck->u.set;
	for (resultset = 0; i < SETSIZE; i++)
	    if (set & ((int64_t)1 << i)) {
		env->stck = INTEGER_NEWNODE(i, save);
		exeterm(env, prog);
		CHECKSTACK("map");
		resultset |= ((int64_t)1 << env->stck->u.num);
	    }
	env->stck = SET_NEWNODE(resultset, save);
	break;
    default:
	BADAGGREGATE("map");
    }
}
#endif
