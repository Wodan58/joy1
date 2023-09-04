/*
    module  : gc.c
    version : 1.3
    date    : 09/04/23
*/
#ifndef GC_C
#define GC_C

/**
OK 3010  gc  :  ->
Initiates garbage collection.
*/
PRIVATE void gc_(pEnv env) { GC_gcollect(); }



#endif
