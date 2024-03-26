/*
    module  : gc.c
    version : 1.6
    date    : 03/21/24
*/
#ifndef GC_C
#define GC_C

/**
IGNORE_OK  3010  gc  :  ->
[IMPURE] Initiates garbage collection.
*/
void gc_(pEnv env)
{
    GC_gcollect();
}
#endif
