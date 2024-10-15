/*
    module  : step.c
    version : 1.11
    date    : 10/11/24
*/
#ifndef STEP_C
#define STEP_C

/**
Q1  OK  2770  step  :  A [P]  ->  ...
Sequentially putting members of aggregate A onto stack,
executes P for each member of A.
*/
void step_(pEnv env)
{
    int i = 0;
    uint64_t set;
    char *str, *volatile ptr;
    Node *prog, *data, *my_dump;

    TWOPARAMS("step");
    ONEQUOTE("step");
    prog = env->stck->u.lis;
    POP(env->stck);
    data = env->stck;
    POP(env->stck);
    switch (data->op) {
    case LIST_:
	for (my_dump = data->u.lis; my_dump; my_dump = my_dump->next) {
	    GNULLARY(my_dump);
	    exeterm(env, prog);
	}
	break;
    case STRING_:
	for (str = ptr = data->u.str; *str; str++) {
	    env->stck = CHAR_NEWNODE(*str, env->stck);
	    exeterm(env, prog);
	}
	break;
    case SET_:
	for (set = data->u.set; i < SETSIZE; i++)
	    if (set & ((int64_t)1 << i)) {
		env->stck = INTEGER_NEWNODE(i, env->stck);
		exeterm(env, prog);
	    }
	break;
    default:
	BADAGGREGATE("step");
    }
}
#endif
