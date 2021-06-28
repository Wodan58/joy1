/*
    module  : uncons.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef UNCONS_C
#define UNCONS_C

/**
2120  uncons  :  A  ->  F R
F and R are the first and the rest of non-empty aggregate A.
*/
PRIVATE void uncons_(pEnv env)
{
    Node *save;

    ONEPARAM("uncons");
    switch (env->stck->op) {
    case SET_: {
        int i = 0;
        long_t set = env->stck->u.set;
        CHECKEMPTYSET(set, "uncons");
        while (!(set & ((long_t)1 << i)))
            i++;
        UNARY(INTEGER_NEWNODE, i);
        NULLARY(SET_NEWNODE, set & ~((long_t)1 << i));
        break;
    }
    case STRING_: {
        char *s = env->stck->u.str;
        CHECKEMPTYSTRING(s, "uncons");
        UNARY(CHAR_NEWNODE, *s);
        NULLARY(STRING_NEWNODE, GC_strdup(++s));
        break;
    }
    case LIST_:
        save = env->stck;
        CHECKEMPTYLIST(env->stck->u.lis, "uncons");
        GUNARY(env->stck->u.lis->op, env->stck->u.lis->u);
        NULLARY(LIST_NEWNODE, save->u.lis->next);
        return;
    default:
        BADAGGREGATE("uncons");
    }
}
#endif
