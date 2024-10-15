/*
    module  : construct.c
    version : 1.8
    date    : 10/11/24
*/
#ifndef CONSTRUCT_C
#define CONSTRUCT_C

/**
Q2  OK  2470  construct  :  [P] [[P1] [P2] ..]  ->  R1 R2 ..
Saves state of stack and then executes [P].
Then executes each [Pi] to give Ri pushed onto saved stack.
*/
void construct_(pEnv env)
{	/*  [P] [[P1] [P2] ..]  ->  X1 X2 ..  */
    Node *prog[2], *save[2];

    TWOPARAMS("construct");
    TWOQUOTES("construct");
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    save[0] = env->stck;	/* save old stack */
    exeterm(env, prog[0]);	/* [P]		  */
    save[1] = env->stck;	/* save new stack */
    while (prog[1]) {
	exeterm(env, prog[1]->u.lis);
	save[0] = newnode2(env, env->stck, save[0]);	/* save result */
	prog[1] = prog[1]->next;
	env->stck = save[1];	/* restore new stack */
    }
    env->stck = save[0];	/* restore old stack */
}
#endif
