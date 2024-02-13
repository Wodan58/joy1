/*
    module  : treerecaux.c
    version : 1.4
    date    : 02/01/24
*/
#ifndef TREERECAUX_C
#define TREERECAUX_C

/**
OK 3260  #treerec  :  T [[O] C]  ->  ...
T is a tree. If T is a leaf, executes O. Else executes [[[O] C] treerec] C.
*/
PRIVATE void treerecaux_(pEnv env)
{
    Node *temp;

    if (env->stck->next->op == LIST_) {
	temp = ANON_FUNCT_NEWNODE(treerecaux_, 0);
	NULLARY(LIST_NEWNODE, temp);
	cons_(env); /*  D  [[[O] C] ANON_FUNCT_]	*/
	exeterm(env, env->stck->u.lis->u.lis->next);
    } else {
	temp = env->stck;
	POP(env->stck);
	exeterm(env, temp->u.lis->u.lis);
    }
}
#endif
