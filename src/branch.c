/*
    module  : branch.c
    version : 1.5
    date    : 09/17/24
*/
#ifndef BRANCH_C
#define BRANCH_C

/**
Q2  OK  2590  branch  :  B [T] [F]  ->  ...
If B is true, then executes T else executes F.
*/
void branch_(pEnv env)
{
    int num;
    Node *second, *third;

    THREEPARAMS("branch");
    TWOQUOTES("branch");
    third = env->stck->u.lis;
    POP(env->stck);
    second = env->stck->u.lis;
    POP(env->stck);
    num = env->stck->u.num;
    POP(env->stck);
    exeterm(env, num ? second : third);
}
#endif
