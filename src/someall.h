/*
    module  : someall.h
    version : 1.10
    date    : 11/11/24
*/
#ifndef SOMEALL_H
#define SOMEALL_H

#define SOMEALL(PROCEDURE, NAME, INITIAL)				\
    void PROCEDURE(pEnv env)						\
    {									\
	int i = 0, result = 0, end_result = INITIAL;			\
	uint64_t set;							\
	char *str, *volatile ptr;					\
	Node *prog, *my_dump, *save;					\
	TWOPARAMS(NAME);						\
	ONEQUOTE(NAME);							\
	prog = env->stck->u.lis;					\
	POP(env->stck);							\
	save = env->stck->next;						\
	switch (env->stck->op) {					\
	case SET_:							\
	    for (set = env->stck->u.set; i < SETSIZE; i++)		\
		if (set & ((int64_t)1 << i)) {				\
		    env->stck = INTEGER_NEWNODE(i, save);		\
		    exeterm(env, prog);					\
		    CHECKSTACK(NAME);					\
		    result = get_boolean(env, env->stck);		\
		    if (result != INITIAL) {				\
			end_result = 1 - INITIAL;			\
			break;						\
		    }							\
		}							\
	    break;							\
	case STRING_:							\
	    for (str = ptr = env->stck->u.str; *str; str++) {		\
		env->stck = CHAR_NEWNODE(*str, save);			\
		exeterm(env, prog);					\
		CHECKSTACK(NAME);					\
		result = get_boolean(env, env->stck);			\
		if (result != INITIAL) {				\
		    end_result = 1 - INITIAL;				\
		    break;						\
		}							\
	    }								\
	    break;							\
	case LIST_:							\
	    my_dump = env->stck->u.lis;					\
	    for (; my_dump; my_dump = my_dump->next) {			\
		env->stck = newnode2(env, my_dump, save);		\
		exeterm(env, prog);					\
		CHECKSTACK(NAME);					\
		result = get_boolean(env, env->stck);			\
		if (result != INITIAL) {				\
		    end_result = 1 - INITIAL;				\
		    break;						\
		}							\
	    }								\
	    break;							\
	default:							\
	    BADAGGREGATE(NAME);						\
	}								\
	env->stck = BOOLEAN_NEWNODE(end_result, save);			\
    }
#endif
