/*
    module  : setecho.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef SETECHO_C
#define SETECHO_C

/**
3020  setecho  :  I  ->
Sets value of echo flag for listing.
I = 0: no echo, 1: echo, 2: with tab, 3: and linenumber.
*/
USETOP(setecho_, "setecho", NUMERICTYPE, echoflag = nodevalue(env->stck).num)



#endif
