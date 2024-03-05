/*
    module  : step.c
    version : 1.7
    date    : 03/05/24
*/
#ifndef STEP_C
#define STEP_C

/**
OK 2770  step  :  A [P]  ->  ...
Sequentially putting members of aggregate A onto stack,
executes P for each member of A.
*/
PRIVATE void step_(pEnv env)
{
    int i;
    uint64_t set;
    char *str, *volatile ptr;
    Node *program, *data, *my_dump;

    TWOPARAMS("step");
    ONEQUOTE("step");
    program = env->stck->u.lis;
    POP(env->stck);
    data = env->stck;
    POP(env->stck);
    switch (data->op) {
    case LIST_:
        my_dump = data->u.lis;
        while (my_dump) {
            GNULLARY(my_dump->op, my_dump->u);
            exeterm(env, program);
            my_dump = my_dump->next;
        }
        break;
    case STRING_:
	ptr = data->u.str; /* remember this */
        for (str = ptr; *str != '\0'; str++) {
            env->stck = CHAR_NEWNODE(*str, env->stck);
            exeterm(env, program);
        }
        break;
    case SET_:
        set = data->u.set;
        for (i = 0; i < SETSIZE; i++)
            if (set & ((int64_t)1 << i)) {
                env->stck = INTEGER_NEWNODE(i, env->stck);
                exeterm(env, program);
            }
        break;
    default:
        BADAGGREGATE("step");
    }
}
#endif
