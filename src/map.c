/*
    module  : map.c
    version : 1.7
    date    : 03/05/24
*/
#ifndef MAP_C
#define MAP_C

/**
OK 2790  map  :  A [P]  ->  B
Executes P on each member of aggregate A,
collects results in sametype aggregate B.
*/
PRIVATE void map_(pEnv env)
{
    Node *program, *my_dump1 = 0, /* step */
                   *my_dump2 = 0, /* head */
            *save, *my_dump3 = 0; /* last */
    int i = 0;
    char *volatile ptr;
    uint64_t set, resultset;
    char *str = 0, *resultstr;

    TWOPARAMS("map");
    ONEQUOTE("map");
    program = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case LIST_:
        my_dump1 = env->stck->u.lis;
        while (my_dump1) {
            env->stck = newnode(env, my_dump1->op, my_dump1->u, save);
            exeterm(env, program);
            CHECKSTACK("map");
            if (!my_dump2) { /* first */
                my_dump2 = newnode(env, env->stck->op, env->stck->u, 0);
                my_dump3 = my_dump2;
            } else { /* further */
                my_dump3->next
                    = newnode(env, env->stck->op, env->stck->u, 0);
                my_dump3 = my_dump3->next;
            }
            my_dump1 = my_dump1->next;
        }
        env->stck = LIST_NEWNODE(my_dump2, save);
        break;
    case STRING_:
        ptr = env->stck->u.str; /* remember this */
        resultstr = GC_malloc_atomic(strlen(ptr) + 1);
        for (str = ptr; *str; str++) {
            env->stck = CHAR_NEWNODE(*str, save);
            exeterm(env, program);
            CHECKSTACK("map");
            resultstr[i++] = env->stck->u.num;
        }
        resultstr[i] = 0;
        env->stck = STRING_NEWNODE(resultstr, save);
        break;
    case SET_:
        set = env->stck->u.set;
        for (resultset = i = 0; i < SETSIZE; i++)
            if (set & ((int64_t)1 << i)) {
                env->stck = INTEGER_NEWNODE(i, save);
                exeterm(env, program);
                CHECKSTACK("map");
                resultset |= ((int64_t)1 << env->stck->u.num);
            }
        env->stck = SET_NEWNODE(resultset, save);
        break;
    default:
        BADAGGREGATE("map");
    }
}
#endif
