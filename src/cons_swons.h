/*
    module  : cons_swons.h
    version : 1.9
    date    : 06/21/24
*/
#ifndef CONS_SWONS_H
#define CONS_SWONS_H

#define CONS_SWONS(PROCEDURE, NAME, AGGR, ELEM)				\
    void PROCEDURE(pEnv env)						\
    {									\
	Index temp;							\
	char *str;							\
	TWOPARAMS(NAME);						\
	switch (nodetype(AGGR)) {					\
	case LIST_:							\
	    temp = newnode2(env, ELEM, nodevalue(AGGR).lis);		\
	    BINARY(LIST_NEWNODE, temp);					\
	    break;							\
	case SET_:							\
	    CHECKSETMEMBER(ELEM, NAME);					\
	    BINARY(SET_NEWNODE, nodevalue(AGGR).set |			\
		  ((int64_t)1 << nodevalue(ELEM).num));			\
	    break;							\
	case STRING_:							\
	    CHECKCHARACTER(ELEM, NAME);					\
	    str = GC_malloc_atomic(strlen(nodevalue(AGGR).str) + 2);	\
	    str[0] = (char)nodevalue(ELEM).num;				\
	    strcpy(str + 1, nodevalue(AGGR).str);			\
	    BINARY(STRING_NEWNODE, str);				\
	    break;							\
	default:							\
	    BADAGGREGATE(NAME);						\
	}								\
    }
#endif
