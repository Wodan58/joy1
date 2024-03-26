/*
    module  : take.c
    version : 1.9
    date    : 03/21/24
*/
#ifndef TAKE_C
#define TAKE_C

/**
OK 2140  take  :  A N  ->  B
Aggregate B is the result of retaining just the first N elements of A.
*/
void take_(pEnv env)
{
    int i, n;
    uint64_t resultset;
    char *old, *ptr, *result;

    Node *my_dump1 = 0; /* old  */
    Node *my_dump2 = 0; /* head */
    Node *my_dump3 = 0; /* last */

    TWOPARAMS("take");
    POSITIVEINDEX(env->stck, "take");
    n = env->stck->u.num;
    switch (env->stck->next->op) {
    case SET_:
        for (resultset = i = 0; i < SETSIZE; i++)
            if (env->stck->next->u.set & ((int64_t)1 << i)) {
                if (n > 0) {
                    --n;
                    resultset |= ((int64_t)1 << i);
                } else
                    break;
            }
        BINARY(SET_NEWNODE, resultset);
        break;
    case STRING_:
        old = env->stck->next->u.str;
        POP(env->stck);
        /* do not swap the order of the next two statements ! ! ! */
#if 0
        if (n < 0)
            n = 0;
#endif
        if (n >= (int)strlen(old))
            return; /* the old string unchanged */
        ptr = result = GC_malloc_atomic(n + 1);
        while (n-- > 0)
            *ptr++ = *old++;
        *ptr = 0;
        UNARY(STRING_NEWNODE, result);
        break;
    case LIST_:
#if 0
        if (n < 1) {
            BINARY(LIST_NEWNODE, 0);
            return;
        } /* null string */
#endif
        my_dump1 = env->stck->next->u.lis;
        while (my_dump1 && n-- > 0) {
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
        break;
    default:
        BADAGGREGATE("take");
    }
}
#endif
