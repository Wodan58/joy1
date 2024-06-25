/*
    module  : dipped.h
    version : 1.4
    date    : 06/21/24
*/
#ifndef DIPPED_H
#define DIPPED_H

#define DIPPED(PROCEDURE, NAME, PARAMCOUNT, ARGUMENT)			\
    void PROCEDURE(pEnv env)						\
    {									\
	Node *save;							\
	PARAMCOUNT(NAME);						\
	save = env->stck;						\
	POP(env->stck);							\
	ARGUMENT(env);							\
	GNULLARY(save);							\
    }
#endif
