/*
    module  : _genrec.c
    version : 1.1
    date    : 08/26/23
*/
#ifndef _GENREC_C
#define _GENREC_C

/**
OK 3170  (genrec)  :  [B] [T] [R1] [R2]  ->  ...
Executes B, if that yields true, executes T.
Else executes R1 and then [[[B] [T] [R1] R2] genrec] R2.
*/
PRIVATE void _genrec_(pEnv env)
{
    int result;
    Node *program, *save, *temp;

    program = env->stck;
    POP(env->stck);
    save = env->stck;
    exeterm(env, program->u.lis->u.lis);		/*	[B]	*/
    CHECKSTACK("genrec");
    result = env->stck->u.num;
    env->stck = save;
    if (result)
	exeterm(env, program->u.lis->next->u.lis);	/*	[T]	*/
    else {
	exeterm(env, program->u.lis->next->next->u.lis);/*	[R1]	*/
	NULLARY(LIST_NEWNODE, program->u.lis);
	temp = ANON_FUNCT_NEWNODE(_genrec_, 0);
	NULLARY(LIST_NEWNODE, temp);
	cons_(env);
	exeterm(env, program->u.lis->next->next->next);	/*	[R2]	*/
    }
}
#endif
