/*
    module  : tailrec.c
    version : 1.8
    date    : 10/11/24
*/
#ifndef TAILREC_C
#define TAILREC_C

/**
Q3  OK  2720  tailrec  :  [P] [T] [R1]  ->  ...
Executes P. If that yields true, executes T.
Else executes R1, recurses.
*/
static void tailrecaux(pEnv env, Node *prog[])
{
    Node *save;
    int result;

tailrec:
    save = env->stck;
    exeterm(env, prog[0]);
    CHECKSTACK("tailrec");
    result = env->stck->u.num;
    env->stck = save;
    if (result)
	exeterm(env, prog[1]);
    else {
	exeterm(env, prog[2]);
	goto tailrec;
    }
}

void tailrec_(pEnv env)
{
    Node *prog[3];

    THREEPARAMS("tailrec");
    THREEQUOTES("tailrec");
    prog[2] = env->stck->u.lis;
    POP(env->stck);
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    tailrecaux(env, prog);
}
#endif
