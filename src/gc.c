/*
    module  : gc.c
    version : 1.5
    date    : 01/22/24
*/
#ifndef GC_C
#define GC_C

/**
OK 3010  gc  :  ->
[IMPURE] Initiates garbage collection.
*/
PRIVATE void gc_(pEnv env)
{
    if (env->ignore)
	return;
    GC_gcollect();
}
#endif
