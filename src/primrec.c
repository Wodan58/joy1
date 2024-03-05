/*
    module  : primrec.c
    version : 1.7
    date    : 03/05/24
*/
#ifndef PRIMREC_C
#define PRIMREC_C

/**
OK 2820  primrec  :  X [I] [C]  ->  R
Executes I to obtain an initial value R0.
For integer X uses increasing positive integers to X, combines by C for new R.
For aggregate X uses successive members and combines by C for new R.
*/
PRIVATE void primrec_(pEnv env)
{
    char *volatile ptr;
    uint64_t i, n = 0, set;
    Node *data, *second, *third, *current;

    THREEPARAMS("primrec");
    TWOQUOTES("primrec");
    third = env->stck->u.lis;
    POP(env->stck);
    second = env->stck->u.lis;
    POP(env->stck);
    data = env->stck;
    POP(env->stck);
    switch (data->op) {
    case LIST_:
        current = data->u.lis;
        while (current) {
            env->stck = newnode(env, current->op, current->u, env->stck);
            current = current->next;
            n++;
        }
        break;
    case STRING_:
        ptr = data->u.str; /* remember this */
        for (i = 0; ptr[i]; i++) {
            env->stck = CHAR_NEWNODE(ptr[i], env->stck);
            n++;
        }
        break;
    case SET_:
        set = data->u.set;
        for (i = 0; i < SETSIZE; i++)
            if (set & ((int64_t)1 << i)) {
                env->stck = INTEGER_NEWNODE(i, env->stck);
                n++;
            }
        break;
    case INTEGER_:
        for (i = data->u.num; i > 0; i--) {
            env->stck = INTEGER_NEWNODE(i, env->stck);
            n++;
        }
        break;
    default:
        BADDATA("primrec");
    }
    exeterm(env, second);
    for (i = 1; i <= n; i++)
        exeterm(env, third);
}
#endif
