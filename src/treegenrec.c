/*
    module  : treegenrec.c
    version : 1.5
    date    : 08/13/23
*/
#ifndef TREEGENREC_C
#define TREEGENREC_C

/**
OK 2910  treegenrec  :  T [O1] [O2] [C]  ->  ...
T is a tree. If T is a leaf, executes O1.
Else executes O2 and then [[[O1] [O2] C] treegenrec] C.
*/
PRIVATE void treegenrecaux(pEnv env)
{
    Node *save, *temp;

    save = env->stck;
    POP(env->stck);
    CHECKSTACK("treegenrec");
    if (env->stck->op == LIST_) {
        exeterm(env, save->u.lis->next->u.lis); /*	[O2]	*/
        GNULLARY(save->op, save->u);
        temp = ANON_FUNCT_NEWNODE(treegenrecaux, 0);
        NULLARY(LIST_NEWNODE, temp);
        cons_(env);
        exeterm(env, env->stck->u.lis->u.lis->next->next); /*	[C]	*/
    } else
        exeterm(env, save->u.lis->u.lis); /*	[O1]	*/
}

PRIVATE void treegenrec_(pEnv env)
{ /* T [O1] [O2] [C]	*/
    FOURPARAMS("treegenrec");
    THREEQUOTES("treegenrec");
    cons_(env);
    cons_(env);
    treegenrecaux(env);
}
#endif
