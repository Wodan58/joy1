/*
    module  : someall.h
    version : 1.6
    date    : 03/21/24
*/
#ifndef SOMEALL_H
#define SOMEALL_H

#define SOMEALL(PROCEDURE, NAME, INITIAL)				\
    void PROCEDURE(pEnv env)						\
    {									\
	int j, result = INITIAL;					\
	uint64_t set;							\
	char *str, *volatile ptr;					\
	Node *program, *my_dump, *save;					\
	TWOPARAMS(NAME);						\
	ONEQUOTE(NAME);							\
	program = env->stck->u.lis;					\
	POP(env->stck);							\
	save = env->stck->next;						\
	switch (env->stck->op) {					\
	case SET_:							\
	    set = env->stck->u.set;					\
	    for (j = 0; j < SETSIZE && result == INITIAL; j++)		\
		if (set & ((int64_t)1 << j)) {				\
		    env->stck = INTEGER_NEWNODE(j, save);		\
		    exeterm(env, program);				\
		    CHECKSTACK(NAME);					\
		    if (env->stck->u.num != INITIAL)			\
			result = 1 - INITIAL;				\
		}							\
	    break;							\
	case STRING_:							\
	    ptr = GC_strdup(env->stck->u.str);				\
	    for (str = ptr; *str != '\0' && result == INITIAL; str++) {	\
		env->stck = CHAR_NEWNODE(*str, save);			\
		exeterm(env, program);					\
		CHECKSTACK(NAME);					\
		if (env->stck->u.num != INITIAL)			\
		    result = 1 - INITIAL;				\
	    }								\
	    break;							\
	case LIST_:							\
	    my_dump = env->stck->u.lis;					\
	    while (my_dump && result == INITIAL) {			\
		env->stck = newnode(env, my_dump->op, my_dump->u, save);\
		exeterm(env, program);					\
		CHECKSTACK(NAME);					\
		if (env->stck->u.num != INITIAL)			\
		    result = 1 - INITIAL;				\
		my_dump = my_dump->next;				\
	    }								\
	    break;							\
	default:							\
	    BADAGGREGATE(NAME);						\
	}								\
	env->stck = BOOLEAN_NEWNODE(result, save);			\
    }
#endif
