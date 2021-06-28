/*
    module  : take.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef TAKE_C
#define TAKE_C

/**
2150  take  :  A N  ->  B
Aggregate B is the result of retaining just the first N elements of A.
*/
PRIVATE void take_(pEnv env)
{
    int n = env->stck->u.num;
    Node *my_dump1 = 0; /* old  */
    Node *my_dump2 = 0; /* head */
    Node *my_dump3 = 0; /* last */

    TWOPARAMS("take");
    switch (env->stck->next->op) {
    case SET_: {
        int i;
        long_t result = 0;
        for (i = 0; i < SETSIZE; i++)
            if (env->stck->next->u.set & ((long_t)1 << i)) {
                if (n > 0) {
                    --n;
                    result |= ((long_t)1 << i);
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
        if (i < 0)
            i = 0;
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
        if (i < 1) {
            BINARY(LIST_NEWNODE, NULL);
            return;
        } /* null string */
        my_dump1 = env->stck->next->u.lis;
        while (my_dump1 != NULL && i-- > 0) {
            if (my_dump2 == NULL) /* first */
            {
                my_dump2 = newnode(env, my_dump1->op, my_dump1->u, NULL);
                my_dump3 = my_dump2;
            } else /* further */
            {
                my_dump3->next = newnode(env, my_dump1->op, my_dump1->u, NULL);
                my_dump3 = my_dump3->next;
            }
            my_dump1 = my_dump1->next;
        }
        my_dump3->next = NULL;
        BINARY(LIST_NEWNODE, my_dump2);
        return;
    }
    default:
        BADAGGREGATE("take");
    }
}
#endif
