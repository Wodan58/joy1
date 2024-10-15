/*
    module  : treestep.c
    version : 1.8
    date    : 10/11/24
*/
#ifndef TREESTEP_C
#define TREESTEP_C

/**
Q1  OK  2870  treestep  :  T [P]  ->  ...
Recursively traverses leaves of tree T, executes P for each leaf.
*/
static void treestepaux(pEnv env, Node *item, Node *prog)
{
    Node *my_dump;

    if (item->op != LIST_) {
	GNULLARY(item);
	exeterm(env, prog);
    } else
	for (my_dump = item->u.lis; my_dump; my_dump = my_dump->next)
	    treestepaux(env, my_dump, prog);
}

void treestep_(pEnv env)
{
    Node *prog, *item;

    TWOPARAMS("treestep");
    ONEQUOTE("treestep");
    prog = env->stck->u.lis;
    POP(env->stck);
    item = env->stck;
    POP(env->stck);
    treestepaux(env, item, prog);
}
#endif
