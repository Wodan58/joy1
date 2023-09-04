/*
    module  : _treegenrec.c
    version : 1.2
    date    : 09/04/23
*/
#ifndef _TREEGENREC_C
#define _TREEGENREC_C

/**
OK 3150  (treegenrec)  :  T [O1] [O2] [C]  ->  ...
T is a tree. If T is a leaf, executes O1.
Else executes O2 and then [[[O1] [O2] C] treegenrec] C.
*/
PRIVATE void _treegenrec_(pEnv env)
{
    Node *save, *temp;

    save = env->stck;
    POP(env->stck);
    CHECKSTACK("treegenrec");
    if (env->stck->op == LIST_) {
	exeterm(env, save->u.lis->next->u.lis); /*	[O2]	*/
	GNULLARY(save->op, save->u);
	temp = ANON_FUNCT_NEWNODE(_treegenrec_, 0);
	NULLARY(LIST_NEWNODE, temp);
	cons_(env);
	exeterm(env, env->stck->u.lis->u.lis->next->next); /*	[C]	*/
    } else
	exeterm(env, save->u.lis->u.lis); /*	[O1]	*/
}
#endif
