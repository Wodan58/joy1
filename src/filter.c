/*
    module  : filter.c
    version : 1.8
    date    : 03/21/24
*/
#ifndef FILTER_C
#define FILTER_C

/**
OK 2830  filter  :  A [B]  ->  A1
Uses test B to filter aggregate A producing sametype aggregate A1.
*/
void filter_(pEnv env)
{
    Node *program, *my_dump1 = 0, /* step */
                   *my_dump2 = 0, /* head */
            *save, *my_dump3 = 0; /* last */
    int i = 0;
    char *volatile ptr;
    char *str, *resultstr;
    uint64_t set, resultset;

    TWOPARAMS("filter");
    ONEQUOTE("filter");
    program = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case SET_:
        set = env->stck->u.set;
        for (resultset = i = 0; i < SETSIZE; i++)
            if (set & ((int64_t)1 << i)) {
                env->stck = INTEGER_NEWNODE(i, save);
                exeterm(env, program);
                CHECKSTACK("filter");
                if (env->stck->u.num)
                    resultset |= ((int64_t)1 << i);
            }
        env->stck = SET_NEWNODE(resultset, save);
        break;
    case STRING_:
        ptr = env->stck->u.str;	/* remember this */
        resultstr = GC_malloc_atomic(strlen(ptr) + 1);
        for (str = ptr; *str; str++) {
            env->stck = CHAR_NEWNODE(*str, save);
            exeterm(env, program);
            CHECKSTACK("filter");
            if (env->stck->u.num)
                resultstr[i++] = *str;
        }
        resultstr[i] = 0;
        env->stck = STRING_NEWNODE(resultstr, save);
        break;
    case LIST_:
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
                    my_dump3->next = newnode(env, my_dump1->op, my_dump1->u, 0);
                    my_dump3 = my_dump3->next;
                }
            }
            my_dump1 = my_dump1->next;
        }
        env->stck = LIST_NEWNODE(my_dump2, save);
        break;
    default:
        BADAGGREGATE("filter");
    }
}
#endif
