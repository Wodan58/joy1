/*
    module  : branch.c
    version : 1.6
    date    : 10/11/24
*/
#ifndef BRANCH_C
#define BRANCH_C

/**
Q2  OK  2590  branch  :  B [T] [F]  ->  ...
If B is true, then executes T else executes F.
*/
void branch_(pEnv env)
{
    int result;
    Node *prog[2];

    THREEPARAMS("branch");
    TWOQUOTES("branch");
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    result = env->stck->u.num;
    POP(env->stck);
    exeterm(env, result ? prog[0] : prog[1]);
}
#endif
