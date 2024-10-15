/*
    module  : unary3.c
    version : 1.7
    date    : 10/11/24
*/
#ifndef UNARY3_C
#define UNARY3_C

/**
Q1  OK  2510  unary3  :  X1 X2 X3 [P]  ->  R1 R2 R3
Executes P three times, with Xi, returns Ri (i = 1..3).
*/
void unary3_(pEnv env)
{	/*  X Y Z [P]  unary3  ==>  X' Y' Z'  */
    Node *prog, *second, *third, *save, *result[3];

    FOURPARAMS("unary3");
    ONEQUOTE("unary3");
    prog = env->stck->u.lis;
    POP(env->stck);
    third = env->stck;
    POP(env->stck);
    second = env->stck;
    POP(env->stck);					/* just X on top */
    save = env->stck->next;
    exeterm(env, prog);					/* execute P */
    result[0] = env->stck;				/* save P(X) */
    env->stck = second;
    env->stck->next = save;				/* just Y on top */
    exeterm(env, prog);					/* execute P */
    result[1] = env->stck;				/* save P(Y) */
    env->stck = third;
    env->stck->next = save;				/* just Z on top */
    exeterm(env, prog);					/* execute P */
    result[2] = env->stck;				/* save P(Z) */
    env->stck = newnode2(env, result[0], save);		/* X' */
    env->stck = newnode2(env, result[1], env->stck);	/* Y' */
    env->stck = newnode2(env, result[2], env->stck);	/* Z' */
}
#endif
