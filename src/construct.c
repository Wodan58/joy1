/*
    module  : construct.c
    version : 1.7
    date    : 09/17/24
*/
#ifndef CONSTRUCT_C
#define CONSTRUCT_C

/**
Q2  OK  2470  construct  :  [P] [[P1] [P2] ..]  ->  R1 R2 ..
Saves state of stack and then executes [P].
Then executes each [Pi] to give Ri pushed onto saved stack.
*/
void construct_(pEnv env)
{ /* [P] [[P1] [P2] ..] -> X1 X2 ..	*/
    Node *first, *second, *save2, *save3;

    TWOPARAMS("construct");
    TWOQUOTES("construct");
    second = env->stck->u.lis;
    POP(env->stck);
    first = env->stck->u.lis;
    POP(env->stck);
    save2 = env->stck; /* save old stack	*/
    exeterm(env, first); /* [P]			*/
    save3 = env->stck; /* save current stack	*/
    while (second) {
	env->stck = save3; /* restore new stack	*/
	exeterm(env, second->u.lis);
	save2 = newnode2(env, env->stck, save2); /* result */
	second = second->next;
    }
    env->stck = save2;
}
#endif
