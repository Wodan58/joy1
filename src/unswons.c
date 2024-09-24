/*
    module  : unswons.c
    version : 1.9
    date    : 09/17/24
*/
#ifndef UNSWONS_C
#define UNSWONS_C

/**
Q0  OK  2120  unswons  :  A  ->  R F
R and F are the rest and the first of non-empty aggregate A.
*/
void unswons_(pEnv env)
{
    Node *save;
    int i = 0;
    char *str;
    uint64_t set;

    ONEPARAM("unswons");
    switch (env->stck->op) {
    case SET_:
	set = env->stck->u.set;
	CHECKEMPTYSET(set, "unswons");
	while (!(set & ((int64_t)1 << i)))
	    i++;
	UNARY(SET_NEWNODE, set & ~((int64_t)1 << i));
	NULLARY(INTEGER_NEWNODE, i);
	break;
    case STRING_:
	str = env->stck->u.str;
	CHECKEMPTYSTRING(str, "unswons");
	UNARY(STRING_NEWNODE, GC_strdup(str + 1));
	NULLARY(CHAR_NEWNODE, *str);
	break;
    case LIST_:
	save = env->stck;
	CHECKEMPTYLIST(env->stck->u.lis, "unswons");
	UNARY(LIST_NEWNODE, env->stck->u.lis->next);
	GNULLARY(save->u.lis);
	break;
    default:
	BADAGGREGATE("unswons");
    }
}
#endif
