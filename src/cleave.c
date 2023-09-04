/*
    module  : cleave.c
    version : 1.3
    date    : 09/04/23
*/
#ifndef CLEAVE_C
#define CLEAVE_C

/**
OK 2580  cleave  :  X [P1] [P2]  ->  R1 R2
Executes P1 and P2, each with X on top, producing two results.
*/
PRIVATE void cleave_(pEnv env)
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
    env->stck = newnode(env, result[0]->op, result[0]->u, save->next); /* X1 */
    env->stck = newnode(env, result[1]->op, result[1]->u, env->stck); /* X2 */
}
#endif
