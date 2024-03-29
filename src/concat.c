/*
    module  : concat.c
    version : 1.7
    date    : 03/21/24
*/
#ifndef CONCAT_C
#define CONCAT_C

/**
OK 2150  concat  :  S T  ->  U
Sequence U is the concatenation of sequences S and T.
*/
void concat_(pEnv env)
{
    char *str;
    Node *my_dump1 = 0; /* old  */
    Node *my_dump2 = 0; /* head */
    Node *my_dump3 = 0; /* last */

    TWOPARAMS("concat");
    SAME2TYPES("concat");
    switch (env->stck->op) {
    case SET_:
        BINARY(SET_NEWNODE, env->stck->next->u.set | env->stck->u.set);
        break;
    case STRING_:
        str = GC_malloc_atomic(strlen(env->stck->next->u.str) +
			       strlen(env->stck->u.str) + 1);
        strcpy(str, env->stck->next->u.str);
        strcat(str, env->stck->u.str);
        BINARY(STRING_NEWNODE, str);
        break;
    case LIST_:
        if (!env->stck->next->u.lis) {
            BINARY(LIST_NEWNODE, env->stck->u.lis);
            return;
        }
        my_dump1 = env->stck->next->u.lis; /* old */
        while (my_dump1) {
            if (!my_dump2) { /* first */
                my_dump2 = newnode(env, my_dump1->op, my_dump1->u, 0);
                my_dump3 = my_dump2;
            } else { /* further */
                my_dump3->next = newnode(env, my_dump1->op, my_dump1->u, 0);
                my_dump3 = my_dump3->next;
            }
            my_dump1 = my_dump1->next;
        }
        my_dump3->next = env->stck->u.lis;
        BINARY(LIST_NEWNODE, my_dump2);
        break;
    default:
        BADAGGREGATE("concat");
    }
}
#endif
