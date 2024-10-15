/*
    module  : primrec.c
    version : 1.11
    date    : 10/11/24
*/
#ifndef PRIMREC_C
#define PRIMREC_C

/**
Q2  OK  2820  primrec  :  X [I] [C]  ->  R
Executes I to obtain an initial value R0.
For integer X uses increasing positive integers to X, combines by C for new R.
For aggregate X uses successive members and combines by C for new R.
*/
void primrec_(pEnv env)
{
    char *volatile ptr;
    uint64_t i, n = 0, set;
    Node *data, *prog[2], *cur;

    THREEPARAMS("primrec");
    TWOQUOTES("primrec");
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    data = env->stck;
    POP(env->stck);
    switch (data->op) {
    case LIST_:
	cur = data->u.lis;
	while (cur) {
	    env->stck = newnode2(env, cur, env->stck);
	    cur = cur->next;
	    n++;
	}
	break;
    case STRING_:
	ptr = data->u.str;	/* remember this */
	for (i = 0; ptr[i]; i++) {
	    env->stck = CHAR_NEWNODE(ptr[i], env->stck);
	    n++;
	}
	break;
    case SET_:
	set = data->u.set;
	for (i = 0; i < SETSIZE; i++)
	    if (set & ((int64_t)1 << i)) {
		env->stck = INTEGER_NEWNODE(i, env->stck);
		n++;
	    }
	break;
    case INTEGER_:
	for (i = data->u.num; i > 0; i--) {
	    env->stck = INTEGER_NEWNODE(i, env->stck);
	    n++;
	}
	break;
    default:
	BADDATA("primrec");
    }
    exeterm(env, prog[0]);
    for (i = 0; i < n; i++)
	exeterm(env, prog[1]);
}
#endif
