/*
    module  : unswons.c
    version : 1.2
    date    : 05/17/22
*/
#ifndef UNSWONS_C
#define UNSWONS_C

/**
2130  unswons  :  A  ->  R F
R and F are the rest and the first of non-empty aggregate A.
*/
PRIVATE void unswons_(pEnv env)
{
    Node *save;

    ONEPARAM("unswons");
    switch (env->stck->op) {
    case SET_: {
        int i = 0;
        long set = env->stck->u.set;
        CHECKEMPTYSET(set, "unswons");
        while (!(set & ((long)1 << i)))
            i++;
        UNARY(SET_NEWNODE, set & ~((long)1 << i));
        NULLARY(INTEGER_NEWNODE, i);
        break;
    }
    case STRING_: {
        char *s = env->stck->u.str, ch;
        CHECKEMPTYSTRING(s, "unswons");
        ch = *s;
        UNARY(STRING_NEWNODE, GC_strdup(s + 1));
        NULLARY(CHAR_NEWNODE, ch);
        break;
    }
    case LIST_:
        save = env->stck;
        CHECKEMPTYLIST(env->stck->u.lis, "unswons");
        UNARY(LIST_NEWNODE, env->stck->u.lis->next);
        GNULLARY(save->u.lis->op, save->u.lis->u);
        return;
    default:
        BADAGGREGATE("unswons");
    }
}
#endif
