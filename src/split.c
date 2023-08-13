/*
    module  : split.c
    version : 1.5
    date    : 08/13/23
*/
#ifndef SPLIT_C
#define SPLIT_C

/**
OK 2860  split  :  A [B]  ->  A1 A2
Uses test B to split aggregate A into sametype aggregates A1 and A2.
*/
PRIVATE void split_(pEnv env)
{
    Node *program, *my_dump1 = 0, /* step */
                   *my_dump2 = 0, /* head */
            *save, *my_dump3 = 0, /* last */
                   *my_dump4 = 0,
                   *my_dump5 = 0;

    TWOPARAMS("split");
    ONEQUOTE("split");
    program = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case SET_: {
        int j;
        uint64_t set = env->stck->u.set, yes_set = 0, no_set = 0;
        for (j = 0; j < SETSIZE; j++) {
            if (set & ((int64_t)1 << j)) {
                env->stck = INTEGER_NEWNODE(j, save);
                exeterm(env, program);
                CHECKSTACK("split");
                if (env->stck->u.num)
                    yes_set |= ((int64_t)1 << j);
                else
                    no_set |= ((int64_t)1 << j);
            }
        }
        env->stck = SET_NEWNODE(yes_set, save);
        NULLARY(SET_NEWNODE, no_set);
        break;
    }
    case STRING_: {
        char *yesstring = 0, *nostring = 0;
        int yesptr = 0, noptr = 0;
        char *volatile ptr = GC_strdup(env->stck->u.str);
        char *str = ptr;
        size_t leng = strlen(str) + 1;
        yesstring = GC_malloc_atomic(leng);
        nostring = GC_malloc_atomic(leng);
        for (; *str; str++) {
            env->stck = CHAR_NEWNODE(*str, save);
            exeterm(env, program);
            CHECKSTACK("split");
            if (env->stck->u.num)
                yesstring[yesptr++] = *str;
            else
                nostring[noptr++] = *str;
        }
        yesstring[yesptr] = 0;
        nostring[noptr] = 0;
        env->stck = STRING_NEWNODE(yesstring, save);
        NULLARY(STRING_NEWNODE, nostring);
        break;
    }
    case LIST_: {
        my_dump1 = env->stck->u.lis;
        while (my_dump1) {
            env->stck = newnode(env, my_dump1->op, my_dump1->u, save);
            exeterm(env, program);
            CHECKSTACK("split");
            if (env->stck->u.num) /* pass */
                if (!my_dump2) { /* first */
                    my_dump2 = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump3 = my_dump2;
                } else { /* further */
                    my_dump3->next
                        = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump3 = my_dump3->next;
                }
            else /* fail */
                if (!my_dump4) { /* first */
                    my_dump4 = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump5 = my_dump4;
                } else { /* further */
                    my_dump5->next = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump5 = my_dump5->next;
                }
            my_dump1 = my_dump1->next;
        }
        env->stck = LIST_NEWNODE(my_dump2, save);
        NULLARY(LIST_NEWNODE, my_dump4);
        break;
    }
    default:
        BADAGGREGATE("split");
    }
}
#endif
