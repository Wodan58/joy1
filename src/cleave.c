/*
    module  : cleave.c
    version : 1.6
    date    : 09/17/24
*/
#ifndef CLEAVE_C
#define CLEAVE_C

/**
Q2  OK  2580  cleave  :  X [P1] [P2]  ->  R1 R2
Executes P1 and P2, each with X on top, producing two results.
*/
void cleave_(pEnv env)
{ /* X [P1] [P2] cleave ==>  X1 X2 */
    Node *program[2], *result[2], *save;

    THREEPARAMS("cleave");
    TWOQUOTES("cleave");
    program[1] = env->stck->u.lis;
    POP(env->stck);
    program[0] = env->stck->u.lis;
    POP(env->stck);
    save = env->stck;
    exeterm(env, program[0]); /* [P1] */
    result[0] = env->stck;
    env->stck = save;
    exeterm(env, program[1]); /* [P2] */
    result[1] = env->stck;
    env->stck = newnode2(env, result[0], save->next); /* X1 */
    env->stck = newnode2(env, result[1], env->stck); /* X2 */
}
#endif
