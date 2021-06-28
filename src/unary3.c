/*
    module  : unary3.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef UNARY3_C
#define UNARY3_C

/**
2530  unary3  :  X1 X2 X3 [P]  ->  R1 R2 R3
Executes P three times, with Xi, returns Ri (i = 1..3).
*/
PRIVATE void unary3_(pEnv env)
{ /*  X Y Z [P]  unary3    ==>  X' Y' Z'	*/
    Node *program, *second, *third, *save, *result[3];

    FOURPARAMS("unary3");
    ONEQUOTE("unary3");
    program = env->stck->u.lis;
    POP(env->stck);
    third = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck); /* just X on top */
    save = env->stck->next;
    exeterm(env, program); /* execute P */
    result[0] = env->stck; /* save P(X) */
    env->stck = second;
    env->stck->next = save; /* just Y on top */
    exeterm(env, program); /* execute P */
    result[1] = env->stck; /* save P(Y) */
    env->stck = third;
    env->stck->next = save; /* just Z on top */
    exeterm(env, program); /* execute P */
    result[2] = env->stck; /* save P(Z) */
    env->stck = newnode(env, result[0]->op, result[0]->u, save); /*  X'	*/
    env->stck
        = newnode(env, result[1]->op, result[1]->u, env->stck); /*  Y'	*/
    env->stck
        = newnode(env, result[2]->op, result[2]->u, env->stck); /*  Z'	*/
}
#endif
