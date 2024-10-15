/*
    module  : treerecaux.c
    version : 1.7
    date    : 10/11/24
*/
#ifndef TREERECAUX_C
#define TREERECAUX_C

#include "cons.c"

/**
Q1  OK  3260  #treerecaux  :  T [[O] C]  ->  ...
T is a tree. If T is a leaf, executes O. Else executes [[[O] C] treerec] C.
*/
void treerecaux_(pEnv env)
{
    Node *temp;

    if (env->stck->next->op == LIST_) {
	temp = ANON_FUNCT_NEWNODE(treerecaux_, 0);
	NULLARY(LIST_NEWNODE, temp);	/* D [[[O] C] ANON_FUNCT_] */
	cons_(env);
	exeterm(env, env->stck->u.lis->u.lis->next);		/* C */
    } else {
	temp = env->stck;
	POP(env->stck);
	exeterm(env, temp->u.lis->u.lis);			/* O */
    }
}
#endif
