/*
    module  : take.c
    version : 1.4
    date    : 05/23/23
*/
#ifndef TAKE_C
#define TAKE_C

/**
2150  take  :  A N  ->  B
Aggregate B is the result of retaining just the first N elements of A.
*/
PRIVATE void take_(pEnv env)
{
    int n;
    Node *my_dump1 = 0; /* old  */
    Node *my_dump2 = 0; /* head */
    Node *my_dump3 = 0; /* last */

    TWOPARAMS("take");
    POSITIVEINDEX(env->stck, "take");
    n = env->stck->u.num;
    switch (env->stck->next->op) {
    case SET_: {
        int i;
        long result = 0;
        for (i = 0; i < SETSIZE; i++)
            if (env->stck->next->u.set & ((long)1 << i)) {
                if (n > 0) {
                    --n;
                    result |= ((long)1 << i);
                } else
                    break;
            }
        BINARY(SET_NEWNODE, result);
        return;
    }
    case STRING_: {
        int i;
        char *old, *p, *result;
        i = env->stck->u.num;
        old = env->stck->next->u.str;
        POP(env->stck);
        /* do not swap the order of the next two statements ! ! ! */
#if 0
        if (i < 0)
            i = 0;
#endif
        if ((size_t)i >= strlen(old))
            return; /* the old string unchanged */
        p = result = GC_malloc_atomic(i + 1);
        while (i-- > 0)
            *p++ = *old++;
        *p = 0;
        UNARY(STRING_NEWNODE, result);
        return;
    }
    case LIST_: {
        int i = env->stck->u.num;
#if 0
        if (i < 1) {
            BINARY(LIST_NEWNODE, 0);
            return;
        } /* null string */
#endif
        my_dump1 = env->stck->next->u.lis;
        while (my_dump1 && i-- > 0) {
            if (!my_dump2) { /* first */
                my_dump2 = newnode(env, my_dump1->op, my_dump1->u, 0);
                my_dump3 = my_dump2;
            } else { /* further */
                my_dump3->next = newnode(env, my_dump1->op, my_dump1->u, 0);
                my_dump3 = my_dump3->next;
            }
            my_dump1 = my_dump1->next;
        }
        if (my_dump3)
            my_dump3->next = 0;
        BINARY(LIST_NEWNODE, my_dump2);
        return;
    }
    default:
        BADAGGREGATE("take");
    }
}
#endif
