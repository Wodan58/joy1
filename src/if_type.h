/*
    module  : if_type.h
    version : 1.4
    date    : 03/21/24
*/
#ifndef IF_TYPE_H
#define IF_TYPE_H

#define IF_TYPE(PROCEDURE, NAME, TYP)					\
    void PROCEDURE(pEnv env)						\
    {									\
	Node *first, *second;						\
	THREEPARAMS(NAME);						\
	TWOQUOTES(NAME);						\
	second = env->stck->u.lis;					\
	POP(env->stck);							\
	first = env->stck->u.lis;					\
	POP(env->stck);							\
	exeterm(env, env->stck->op == TYP ? first : second);		\
    }
#endif
