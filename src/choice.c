/*
    module  : choice.c
    version : 1.3
    date    : 03/21/24
*/
#ifndef CHOICE_C
#define CHOICE_C

/**
OK 1330  choice  :  B T F  ->  X
If B is true, then X = T else X = F.
*/
void choice_(pEnv env)
{
    THREEPARAMS("choice");
    if (nodevalue(nextnode2(env->stck)).num)
        env->stck = newnode(env, nodetype(nextnode1(env->stck)),
            nodevalue(nextnode1(env->stck)), nextnode3(env->stck));
    else
        env->stck = newnode(env, nodetype(env->stck), nodevalue(env->stck),
            nextnode3(env->stck));
}
#endif
