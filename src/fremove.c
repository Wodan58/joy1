/*
    module  : fremove.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef FREMOVE_C
#define FREMOVE_C

/**
1930  fremove  :  P  ->  B
The file system object with pathname P is removed from the file system.
B is a boolean indicating success or failure.
*/
PRIVATE void fremove_(pEnv env)
{
    ONEPARAM("fremove");
    STRING("fremove");
    UNARY(BOOLEAN_NEWNODE, !remove(nodevalue(env->stck).str));
}
#endif
