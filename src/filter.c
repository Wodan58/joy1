/*
    module  : filter.c
    version : 1.6
    date    : 09/04/23
*/
#ifndef FILTER_C
#define FILTER_C

/**
OK 2830  filter  :  A [B]  ->  A1
Uses test B to filter aggregate A producing sametype aggregate A1.
*/
PRIVATE void filter_(pEnv env)
{
    Node *program, *my_dump1 = 0, /* step */
                   *my_dump2 = 0, /* head */
            *save, *my_dump3 = 0; /* last */

    TWOPARAMS("filter");
    ONEQUOTE("filter");
    program = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case SET_: {
        int j;
        uint64_t set = env->stck->u.set, resultset = 0;
        for (j = 0; j < SETSIZE; j++) {
            if (set & ((int64_t)1 << j)) {
                env->stck = INTEGER_NEWNODE(j, save);
                exeterm(env, program);
                CHECKSTACK("filter");
                if (env->stck->u.num)
                    resultset |= ((int64_t)1 << j);
            }
        }
        env->stck = SET_NEWNODE(resultset, save);
        break;
    }
    case STRING_: {
        char *s, *resultstring;
        int j = 0;
        char *volatile ptr = GC_strdup(env->stck->u.str);
        resultstring = GC_malloc_atomic(strlen(ptr) + 1);
        for (s = ptr; *s; s++) {
            env->stck = CHAR_NEWNODE(*s, save);
            exeterm(env, program);
            CHECKSTACK("filter");
            if (env->stck->u.num)
                resultstring[j++] = *s;
        }
        resultstring[j] = 0;
        env->stck = STRING_NEWNODE(resultstring, save);
        break;
    }
    case LIST_: {
        my_dump1 = env->stck->u.lis;
        while (my_dump1) {
            env->stck = newnode(env, my_dump1->op, my_dump1->u, save);
            exeterm(env, program);
            CHECKSTACK("filter");
            if (env->stck->u.num) { /* test */
                if (!my_dump2) { /* first */
                    my_dump2 = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump3 = my_dump2;
                } else { /* further */
                    my_dump3->next
                        = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump3 = my_dump3->next;
                }
            }
            my_dump1 = my_dump1->next;
        }
        env->stck = LIST_NEWNODE(my_dump2, save);
        break;
    }
    default:
        BADAGGREGATE("filter");
    }
}
#endif
