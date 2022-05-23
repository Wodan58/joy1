/*
    module  : map.c
    version : 1.3
    date    : 05/17/22
*/
#ifndef MAP_C
#define MAP_C

/**
2810  map  :  A [P]  ->  B
Executes P on each member of aggregate A,
collects results in sametype aggregate B.
*/
PRIVATE void map_(pEnv env)
{
    Node *program, *my_dump1 = 0, /* step */
                   *my_dump2 = 0, /* head */
            *save, *my_dump3 = 0; /* last */

    TWOPARAMS("map");
    ONEQUOTE("map");
    program = env->stck->u.lis;
    POP(env->stck);
    save = env->stck->next;
    switch (env->stck->op) {
    case LIST_: {
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
    }
    case STRING_: {
        char *s = 0, *resultstring = 0;
        int j = 0;
        char *volatile ptr = GC_strdup(env->stck->u.str);
        resultstring = GC_malloc_atomic(strlen(ptr) + 1);
        for (s = ptr; *s; s++) {
            env->stck = CHAR_NEWNODE(*s, save);
            exeterm(env, program);
            CHECKSTACK("map");
            resultstring[j++] = (char)env->stck->u.num;
        }
        resultstring[j] = 0;
        env->stck = STRING_NEWNODE(resultstring, save);
        break;
    }
    case SET_: {
        int i;
        long set = env->stck->u.set, resultset = 0;
        for (i = 0; i < SETSIZE; i++)
            if (set & ((long)1 << i)) {
                env->stck = INTEGER_NEWNODE(i, save);
                exeterm(env, program);
                CHECKSTACK("map");
                resultset |= ((long)1 << env->stck->u.num);
            }
        env->stck = SET_NEWNODE(resultset, save);
        break;
    }
    default:
        BADAGGREGATE("map");
    }
}
#endif
