/*
    module  : treestep.c
    version : 1.6
    date    : 06/21/24
*/
#ifndef TREESTEP_C
#define TREESTEP_C

/**
OK 2870  treestep  :  T [P]  ->  ...
Recursively traverses leaves of tree T, executes P for each leaf.
*/
void treestepaux(pEnv env, Node *item, Node *program)
{
    Node *my_dump;

    if (item->op != LIST_) {
	GNULLARY(item);
	exeterm(env, program);
    } else
	for (my_dump = item->u.lis; my_dump; my_dump = my_dump->next)
	    treestepaux(env, my_dump, program);
}

void treestep_(pEnv env)
{
    Node *item, *program;

    TWOPARAMS("treestep");
    ONEQUOTE("treestep");
    program = env->stck->u.lis;
    POP(env->stck);
    item = env->stck;
    POP(env->stck);
    treestepaux(env, item, program);
}
#endif
