/*
    module  : if_type.h
    version : 1.5
    date    : 10/11/24
*/
#ifndef IF_TYPE_H
#define IF_TYPE_H

#define IF_TYPE(PROCEDURE, NAME, TYP)					\
    void PROCEDURE(pEnv env)						\
    {									\
	Node *prog[2];							\
	THREEPARAMS(NAME);						\
	TWOQUOTES(NAME);						\
	prog[1] = env->stck->u.lis;					\
	POP(env->stck);							\
	prog[0] = env->stck->u.lis;					\
	POP(env->stck);							\
	exeterm(env, env->stck->op == TYP ? prog[0] : prog[1]);		\
    }
#endif
