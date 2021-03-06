/*
    module  : treerec.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef TREEREC_C
#define TREEREC_C

PRIVATE void treerecaux(pEnv env)
{
    if (env->stck->next->op == LIST_) {
        NULLARY(LIST_NEWNODE, ANON_FUNCT_NEWNODE(treerecaux, NULL));
        cons_(env); /*  D  [[[O] C] ANON_FUNCT_]	*/
        exeterm(env, env->stck->u.lis->u.lis->next);
    } else {
        Node *n = env->stck;
        POP(env->stck);
        exeterm(env, n->u.lis->u.lis);
    }
}

/**
2900  treerec  :  T [O] [C]  ->  ...
T is a tree. If T is a leaf, executes O. Else executes [[[O] C] treerec] C.
*/
PRIVATE void treerec_(pEnv env)
{
    THREEPARAMS("treerec");
    TWOQUOTES("treerec");
    cons_(env);
    treerecaux(env);
}
#endif
