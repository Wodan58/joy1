/*
    module  : divide.c
    version : 1.2
    date    : 05/02/22
*/
#ifndef DIVIDE_C
#define DIVIDE_C

/**
1410  /  :  I J  ->  K
Integer K is the (rounded) ratio of integers I and J.  Also supports float.
*/
PRIVATE void divide_(pEnv env)
{
    TWOPARAMS("/");
    CHECKDIVISOR("/");
    FLOAT_I(/);
    INTEGERS2("/");
    BINARY(INTEGER_NEWNODE,
        nodevalue(nextnode1(env->stck)).num / nodevalue(env->stck).num);
}
#endif
