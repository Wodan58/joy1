/*
    module  : step.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef STEP_C
#define STEP_C

/**
2790  step  :  A [P]  ->  ...
Sequentially putting members of aggregate A onto stack,
executes P for each member of A.
*/
PRIVATE void step_(pEnv env)
{
    Node *program, *data, *my_dump;

    TWOPARAMS("step");
    ONEQUOTE("step");
    program = env->stck->u.lis;
    POP(env->stck);
    data = env->stck;
    POP(env->stck);
    switch (data->op) {
    case LIST_: {
        my_dump = data->u.lis;
        while (my_dump != NULL) {
            GNULLARY(my_dump->op, my_dump->u);
            exeterm(env, program);
            my_dump = my_dump->next;
        }
        break;
    }
    case STRING_: {
        char *s;
        for (s = data->u.str; *s != '\0'; s++) {
            env->stck = CHAR_NEWNODE(*s, env->stck);
            exeterm(env, program);
        }
        break;
    }
    case SET_: {
        int i;
        long_t set = data->u.set;
        for (i = 0; i < SETSIZE; i++)
            if (set & ((long_t)1 << i)) {
                env->stck = INTEGER_NEWNODE(i, env->stck);
                exeterm(env, program);
            }
        break;
    }
    default:
        BADAGGREGATE("step");
    }
}
#endif
