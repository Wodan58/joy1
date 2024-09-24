/*
    module  : uncons.c
    version : 1.9
    date    : 09/17/24
*/
#ifndef UNCONS_C
#define UNCONS_C

/**
Q0  OK  2110  uncons  :  A  ->  F R
F and R are the first and the rest of non-empty aggregate A.
*/
void uncons_(pEnv env)
{
    Node *save;
    int i = 0;
    char *str;
    uint64_t set;

    ONEPARAM("uncons");
    switch (env->stck->op) {
    case SET_:
	set = env->stck->u.set;
	CHECKEMPTYSET(set, "uncons");
	while (!(set & ((int64_t)1 << i)))
	    i++;
	UNARY(INTEGER_NEWNODE, i);
	NULLARY(SET_NEWNODE, set & ~((int64_t)1 << i));
	break;
    case STRING_:
	str = env->stck->u.str;
	CHECKEMPTYSTRING(str, "uncons");
	UNARY(CHAR_NEWNODE, *str);
	NULLARY(STRING_NEWNODE, GC_strdup(++str));
	break;
    case LIST_:
	save = env->stck;
	CHECKEMPTYLIST(env->stck->u.lis, "uncons");
	GUNARY(env->stck->u.lis);
	NULLARY(LIST_NEWNODE, save->u.lis->next);
	break;
    default:
	BADAGGREGATE("uncons");
    }
}
#endif
