/*
    module  : casting.c
    version : 1.7
    date    : 09/26/24
*/
#ifndef CASTING_C
#define CASTING_C

/**
Q0  OK  3150  casting  :  X Y  ->  Z
Z takes the value from X and uses the value from Y as its type.
*/
void casting_(pEnv env)
{
    Node node;

    TWOPARAMS("casting");
    node.op = nodevalue(env->stck).num;
    node.u = nodevalue(nextnode1(env->stck));
    GBINARY(&node);
}
#endif
