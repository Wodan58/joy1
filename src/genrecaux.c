/*
    module  : genrecaux.c
    version : 1.5
    date    : 03/21/24
*/
#ifndef GENRECAUX_C
#define GENRECAUX_C

/**
OK 3240  #genrec  :  [[B] [T] [R1] R2]  ->  ...
Executes B, if that yields true, executes T.
Else executes R1 and then [[[B] [T] [R1] R2] genrec] R2.
*/
void genrecaux_(pEnv env)
{
    int result;
    Node *program, *save, *temp;

    program = env->stck;
    POP(env->stck);
    save = env->stck;
    exeterm(env, program->u.lis->u.lis);		/*	[B]	*/
    CHECKSTACK("genrecaux");
    result = env->stck->u.num;
    env->stck = save;
    if (result)
	exeterm(env, program->u.lis->next->u.lis);	/*	[T]	*/
    else {
	exeterm(env, program->u.lis->next->next->u.lis);/*	[R1]	*/
	NULLARY(LIST_NEWNODE, program->u.lis);
	temp = ANON_FUNCT_NEWNODE(genrecaux_, 0);
	NULLARY(LIST_NEWNODE, temp);
	cons_(env);
	exeterm(env, program->u.lis->next->next->next);	/*	[R2]	*/
    }
}
#endif
