/*
    module  : argv.c
    version : 1.4
    date    : 06/20/22
*/
#ifndef ARGV_C
#define ARGV_C

/**
3060  argv  :  ->  A
Creates an aggregate A containing the interpreter's command line arguments.
*/
PRIVATE void argv_(pEnv env)
{
    int i;
    Index list = 0;

    for (i = env->g_argc - 1; i >= 0; i--)
        list = STRING_NEWNODE(env->g_argv[i], list);
    NULLARY(LIST_NEWNODE, list);
}
#endif
