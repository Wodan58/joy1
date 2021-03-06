/*
    module  : fopen.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef FOPEN_C
#define FOPEN_C

/**
1900  fopen  :  P M  ->  S
The file system object with pathname P is opened with mode M (r, w, a, etc.)
and stream object S is pushed; if the open fails, file:NULL is pushed.
*/
PRIVATE void fopen_(pEnv env)
{
    TWOPARAMS("fopen");
    STRING("fopen");
    STRING2("fopen");
    BINARY(FILE_NEWNODE,
        fopen(nodevalue(nextnode1(env->stck)).str, nodevalue(env->stck).str));
}
#endif
