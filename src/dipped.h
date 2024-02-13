/*
    module  : dipped.h
    version : 1.2
    date    : 02/01/24
*/
#ifndef DIPPED_H
#define DIPPED_H

#define DIPPED(PROCEDURE, NAME, PARAMCOUNT, ARGUMENT)			\
    PRIVATE void PROCEDURE(pEnv env)					\
    {									\
	Node *save;							\
	PARAMCOUNT(NAME);						\
	save = env->stck;						\
	POP(env->stck);							\
	ARGUMENT(env);							\
	GNULLARY(save->op, save->u);					\
    }
#endif
