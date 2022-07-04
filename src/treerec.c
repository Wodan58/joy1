/*
    module  : treerec.c
    version : 1.3
    date    : 06/28/22
*/
#ifndef TREEREC_C
#define TREEREC_C

/**
2900  treerec  :  T [O] [C]  ->  ...
T is a tree. If T is a leaf, executes O. Else executes [[[O] C] treerec] C.
*/
PRIVATE void treerecaux(pEnv env)
{
    Node *temp;
    if (env->stck->next->op == LIST_) {
        temp = ANON_FUNCT_NEWNODE(treerecaux, 0);
        NULLARY(LIST_NEWNODE, temp);
        cons_(env); /*  D  [[[O] C] ANON_FUNCT_]	*/
        exeterm(env, env->stck->u.lis->u.lis->next);
    } else {
        Node *n = env->stck;
        POP(env->stck);
        exeterm(env, n->u.lis->u.lis);
    }
}

PRIVATE void treerec_(pEnv env)
{
    THREEPARAMS("treerec");
    TWOQUOTES("treerec");
    cons_(env);
    treerecaux(env);
}
#endif
