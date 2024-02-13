/*
    module  : n_ary.h
    version : 1.3
    date    : 02/01/24
*/
#ifndef N_ARY_H
#define N_ARY_H

#define N_ARY(PROCEDURE, NAME, PARAMCOUNT, TOP)				\
    PRIVATE void PROCEDURE(pEnv env)					\
    {									\
	Node *save, *top;						\
	PARAMCOUNT(NAME);						\
	ONEQUOTE(NAME);							\
	save = env->stck;						\
	POP(env->stck);							\
	top = TOP;							\
	exeterm(env, save->u.lis);					\
	CHECKVALUE(NAME);						\
	env->stck = newnode(env, env->stck->op, env->stck->u, top);	\
    }
#endif
