/*
    module  : concat.c
    version : 1.9
    date    : 09/17/24
*/
#ifndef CONCAT_C
#define CONCAT_C

/**
Q0  OK  2150  concat  :  S T  ->  U
Sequence U is the concatenation of sequences S and T.
*/
void concat_(pEnv env)
{
    char *str;
    size_t leng;
    Node *my_dump1 = 0; /* old  */
    Node *my_dump2 = 0; /* head */
    Node *my_dump3 = 0; /* last */

    TWOPARAMS("concat");
    SAME2TYPES("concat");
    switch (env->stck->op) {
    case SET_:
	BINARY(SET_NEWNODE, env->stck->next->u.set | env->stck->u.set);
	break;
    case STRING_:
	leng = strlen(nodevalue(nextnode1(env->stck)).str) +
	       strlen(nodevalue(env->stck).str) + 1;
	str = GC_malloc_atomic(leng);
	snprintf(str, leng, "%s%s", nodevalue(nextnode1(env->stck)).str,
				    nodevalue(env->stck).str);
	BINARY(STRING_NEWNODE, str);
	break;
    case LIST_:
	if (!env->stck->next->u.lis) {
	    BINARY(LIST_NEWNODE, env->stck->u.lis);
	    return;
	}
	my_dump1 = env->stck->next->u.lis;
	for (; my_dump1; my_dump1 = my_dump1->next)
	    if (!my_dump2) /* first */
		my_dump3 = my_dump2 = newnode2(env, my_dump1, 0);
	    else  /* further */
		my_dump3 = my_dump3->next = newnode2(env, my_dump1, 0);
	my_dump3->next = env->stck->u.lis;
	BINARY(LIST_NEWNODE, my_dump2);
	break;
    default:
	BADAGGREGATE("concat");
    }
}
#endif
