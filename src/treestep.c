/*
    module  : treestep.c
    version : 1.5
    date    : 03/21/24
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
        GNULLARY(item->op, item->u);
        exeterm(env, program);
    } else {
        my_dump = item->u.lis;
        while (my_dump != NULL) {
            treestepaux(env, my_dump, program);
            my_dump = my_dump->next;
        }
    }
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
