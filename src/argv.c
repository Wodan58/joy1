/*
    module  : argv.c
    version : 1.8
    date    : 06/10/24
*/
#ifndef ARGV_C
#define ARGV_C

/**
OK 3040  argv  :  ->  A
Creates an aggregate A containing the interpreter's command line arguments.
*/
void argv_(pEnv env)
{
    int i;
    Index list = 0;

    for (i = env->g_argc - 1; i >= 0; i--)
	list = STRING_NEWNODE(env->g_argv[i], list);
    NULLARY(LIST_NEWNODE, list);
}
#endif
