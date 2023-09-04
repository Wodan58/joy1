/*
    module  : leql.c
    version : 1.4
    date    : 09/04/23
*/
#ifndef LEQL_C
#define LEQL_C

#include "compare.h"

/**
OK 2240  <=\0leql  :  X Y  ->  B
Either both X and Y are numeric or both are strings or symbols.
Tests whether X less than or equal to Y.  Also supports float.
*/
COMPREL(leql_, "<=", BOOLEAN_NEWNODE, <=, !(i & ~j))


#endif
