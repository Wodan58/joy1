/*
    module  : genrec.c
    version : 1.8
    date    : 01/17/24
*/
#ifndef GENREC_C
#define GENREC_C

/**
OK 2740  genrec  :  [B] [T] [R1] [R2]  ->  ...
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
    genrecaux_(env);
}
#endif
