/*
    module  : _treerec.c
    version : 1.1
    date    : 08/26/23
*/
#ifndef _TREEREC_C
#define _TREEREC_C

/**
OK 3180  (treerec)  :  T [O] [C]  ->  ...
T is a tree. If T is a leaf, executes O. Else executes [[[O] C] treerec] C.
*/
PRIVATE void _treerec_(pEnv env)
{
    Node *temp;

    if (env->stck->next->op == LIST_) {
	temp = ANON_FUNCT_NEWNODE(_treerec_, 0);
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
