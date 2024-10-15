/*
    module  : linrec.c
    version : 1.8
    date    : 10/11/24
*/
#ifndef LINREC_C
#define LINREC_C

/**
Q4  OK  2710  linrec  :  [P] [T] [R1] [R2]  ->  ...
Executes P. If that yields true, executes T.
Else executes R1, recurses, executes R2.
*/
static void linrecaux(pEnv env, Node *prog[])
{
    Node *save;
    int result;

    save = env->stck;
    exeterm(env, prog[0]);
    CHECKSTACK("linrec");
    result = env->stck->u.num;
    env->stck = save;
    if (result)
	exeterm(env, prog[1]);
    else {
	exeterm(env, prog[2]);
	linrecaux(env, prog);
	exeterm(env, prog[3]);
    }
}

void linrec_(pEnv env)
{
    Node *prog[4];

    FOURPARAMS("linrec");
    FOURQUOTES("linrec");
    prog[3] = env->stck->u.lis;
    POP(env->stck);
    prog[2] = env->stck->u.lis;
    POP(env->stck);
    prog[1] = env->stck->u.lis;
    POP(env->stck);
    prog[0] = env->stck->u.lis;
    POP(env->stck);
    linrecaux(env, prog);
}
#endif
