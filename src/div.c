/*
    module  : div.c
    version : 1.3
    date    : 08/13/23
*/
#ifndef DIV_C
#define DIV_C

/**
OK 1430  div  :  I J  ->  K L
Integers K and L are the quotient and remainder of dividing I by J.
*/
PRIVATE void div_(pEnv env)
{
    lldiv_t result;

    TWOPARAMS("div");
    INTEGERS2("div");
    CHECKZERO("div");
    result
        = lldiv(nodevalue(nextnode1(env->stck)).num, nodevalue(env->stck).num);
    BINARY(INTEGER_NEWNODE, result.quot);
    NULLARY(INTEGER_NEWNODE, result.rem);
}
#endif
