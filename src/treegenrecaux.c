/*
    module  : treegenrecaux.c
    version : 1.7
    date    : 09/17/24
*/
#ifndef TREEGENRECAUX_C
#define TREEGENRECAUX_C

/**
Q1  OK  3250  #treegenrec  :  T [[O1] [O2] C]  ->  ...
T is a tree. If T is a leaf, executes O1.
Else executes O2 and then [[[O1] [O2] C] treegenrec] C.
*/
void treegenrecaux_(pEnv env)
{
    Node *save, *temp;

    save = env->stck;
    POP(env->stck);
    CHECKSTACK("treegenrecaux");
    if (env->stck->op == LIST_) {
	exeterm(env, save->u.lis->next->u.lis);	/* [O2] */
	GNULLARY(save);
	temp = ANON_FUNCT_NEWNODE(treegenrecaux_, 0);
	NULLARY(LIST_NEWNODE, temp);
	cons_(env);
	exeterm(env, env->stck->u.lis->u.lis->next->next);	/* [C] */
    } else
	exeterm(env, save->u.lis->u.lis);	/* [O1] */
}
#endif
