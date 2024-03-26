/*
    module  : unary4.c
    version : 1.4
    date    : 03/21/24
*/
#ifndef UNARY4_C
#define UNARY4_C

/**
OK 2520  unary4  :  X1 X2 X3 X4 [P]  ->  R1 R2 R3 R4
Executes P four times, with Xi, returns Ri (i = 1..4).
*/
void unary4_(pEnv env)
{ /*  X Y Z W [P]  unary4    ==>  X' Y' Z' W'	*/
    Node *program, *second, *third, *fourth, *save, *result[4];

    FIVEPARAMS("unary4");
    ONEQUOTE("unary4");
    program = env->stck->u.lis;
    POP(env->stck);
    fourth = env->stck;
    POP(env->stck);
    third = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck); /* just X on top */
    save = env->stck->next;
    exeterm(env, program); /* execute P */
    result[0] = env->stck; /* save p(X) */
    env->stck = second;
    env->stck->next = save; /* just Y on top */
    exeterm(env, program); /* execute P */
    result[1] = env->stck; /* save P(Y) */
    env->stck = third; /* just Z on top */
    env->stck->next = save;
    exeterm(env, program); /* execute P */
    result[2] = env->stck; /* save P(Z) */
    env->stck = fourth; /* just W on top */
    env->stck->next = save;
    exeterm(env, program); /* execute P */
    result[3] = env->stck; /* save P(W) */
    env->stck = newnode(env, result[0]->op, result[0]->u, save); /*  X'	*/
    env->stck
        = newnode(env, result[1]->op, result[1]->u, env->stck); /*  Y'	*/
    env->stck
        = newnode(env, result[2]->op, result[2]->u, env->stck); /*  Z'	*/
    env->stck
        = newnode(env, result[3]->op, result[3]->u, env->stck); /*  W'	*/
}
#endif
