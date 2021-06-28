/*
    module  : genrec.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef GENREC_C
#define GENREC_C

PRIVATE void genrecaux(pEnv env)
{
    int result;
    Node *program, *save;

    program = env->stck;
    POP(env->stck);
    save = env->stck;
    exeterm(env, program->u.lis->u.lis); /*	[I]	*/
    result = env->stck->u.num;
    env->stck = save;
    if (result)
        exeterm(env, program->u.lis->next->u.lis); /*	[T]	*/
    else {
        exeterm(env, program->u.lis->next->next->u.lis); /*	[R1]	*/
        NULLARY(LIST_NEWNODE, program->u.lis);
        NULLARY(LIST_NEWNODE, ANON_FUNCT_NEWNODE(genrecaux, NULL));
        cons_(env);
        exeterm(env, program->u.lis->next->next->next);
    } /*   [R2]	*/
}

/**
2760  genrec  :  [B] [T] [R1] [R2]  ->  ...
Executes B, if that yields true, executes T.
Else executes R1 and then [[[B] [T] [R1] R2] genrec] R2.
*/
PRIVATE void genrec_(pEnv env)
{
    FOURPARAMS("genrec");
    FOURQUOTES("genrec");
    cons_(env);
    cons_(env);
    cons_(env);
    genrecaux(env);
}
#endif
