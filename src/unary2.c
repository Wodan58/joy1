/*
    module  : unary2.c
    version : 1.4
    date    : 03/21/24
*/
#ifndef UNARY2_C
#define UNARY2_C

/**
OK 2500  unary2  :  X1 X2 [P]  ->  R1 R2
Executes P twice, with X1 and X2 on top of the stack.
Returns the two values R1 and R2.
*/
void unary2_(pEnv env)
{ /*   Y  Z  [P]  unary2     ==>  Y'  Z'  */
    Node *program, *second, *save, *result[2];

    THREEPARAMS("unary2");
    ONEQUOTE("unary2");
    program = env->stck->u.lis;
    POP(env->stck);
    second = env->stck;
    POP(env->stck); /* just Y on top */
    save = env->stck->next;
    exeterm(env, program); /* execute P */
    result[0] = env->stck; /* save P(Y) */
    env->stck = second;
    env->stck->next = save; /* just Z on top */
    exeterm(env, program); /* execute P */
    result[1] = env->stck; /* save P(Z) */
    env->stck = newnode(env, result[0]->op, result[0]->u, save); /*  Y'	*/
    env->stck
        = newnode(env, result[1]->op, result[1]->u, env->stck); /*  Z'	*/
}
#endif
