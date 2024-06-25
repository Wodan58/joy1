/*
    module  : n_ary.h
    version : 1.5
    date    : 06/21/24
*/
#ifndef N_ARY_H
#define N_ARY_H

#define N_ARY(PROCEDURE, NAME, PARAMCOUNT, TOP)				\
    void PROCEDURE(pEnv env)						\
    {									\
	Node *save, *top;						\
	PARAMCOUNT(NAME);						\
	ONEQUOTE(NAME);							\
	save = env->stck;						\
	POP(env->stck);							\
	top = TOP;							\
	exeterm(env, save->u.lis);					\
	CHECKVALUE(NAME);						\
	env->stck = newnode2(env, env->stck, top);			\
    }
#endif
