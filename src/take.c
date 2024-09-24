/*
    module  : take.c
    version : 1.11
    date    : 09/17/24
*/
#ifndef TAKE_C
#define TAKE_C

/**
Q0  OK  2140  take  :  A N  ->  B
Aggregate B is the result of retaining just the first N elements of A.
*/
void take_(pEnv env)
{
    char *str;
    int i = 0, n;
    uint64_t set;

    Node *my_dump1 = 0; /* old  */
    Node *my_dump2 = 0; /* head */
    Node *my_dump3 = 0; /* last */

    TWOPARAMS("take");
    POSITIVEINDEX(env->stck, "take");
    n = env->stck->u.num;
    POP(env->stck);
    switch (env->stck->op) {
    case SET_:
	for (set = 0; i < SETSIZE && n; i++)
	    if (env->stck->u.set & ((int64_t)1 << i)) {
		set |= ((int64_t)1 << i);
		n--;
	    }
	UNARY(SET_NEWNODE, set);
	break;
    case STRING_:
	if (n >= (int)strlen(env->stck->u.str))
	    return;	/* the old string unchanged */
	str = GC_strdup(env->stck->u.str);
	str[n] = 0;	/* end the string */
	UNARY(STRING_NEWNODE, str);
	break;
    case LIST_:
	my_dump1 = env->stck->u.lis;
	for (; my_dump1 && n; my_dump1 = my_dump1->next, n--)
	    if (!my_dump2) /* first */
		my_dump3 = my_dump2 = newnode2(env, my_dump1, 0);
	    else /* further */
		my_dump3 = my_dump3->next = newnode2(env, my_dump1, 0);
	UNARY(LIST_NEWNODE, my_dump2);
	break;
    default:
	BADAGGREGATE("take");
    }
}
#endif
