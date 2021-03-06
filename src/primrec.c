/*
    module  : primrec.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef PRIMREC_C
#define PRIMREC_C

/**
2840  primrec  :  X [I] [C]  ->  R
Executes I to obtain an initial value R0.
For integer X uses increasing positive integers to X, combines by C for new R.
For aggregate X uses successive members and combines by C for new R.
*/
PRIVATE void primrec_(pEnv env)
{
    int n = 0;
    int i;
    Node *data, *second, *third;

    THREEPARAMS("primrec");
    TWOQUOTES("primrec");
    third = env->stck->u.lis;
    POP(env->stck);
    second = env->stck->u.lis;
    POP(env->stck);
    data = env->stck;
    POP(env->stck);
    switch (data->op) {
    case LIST_: {
        Node *current = data->u.lis;
        while (current != NULL) {
            env->stck = newnode(env, current->op, current->u, env->stck);
            current = current->next;
            n++;
        }
        break;
    }
    case STRING_: {
        char *s, *volatile ptr;
        for (ptr = s = data->u.str; *s != '\0'; s++) {
            env->stck = CHAR_NEWNODE(*s, env->stck);
            n++;
        }
        break;
    }
    case SET_: {
        int j;
        long_t set = data->u.set;
        for (j = 0; j < SETSIZE; j++)
            if (set & ((long_t)1 << j)) {
                env->stck = INTEGER_NEWNODE(j, env->stck);
                n++;
            }
        break;
    }
    case INTEGER_: {
        long_t j;
        for (j = data->u.num; j > 0; j--) {
            env->stck = INTEGER_NEWNODE(j, env->stck);
            n++;
        }
        break;
    }
    default:
        BADDATA("primrec");
    }
    exeterm(env, second);
    for (i = 1; i <= n; i++)
        exeterm(env, third);
}
#endif
