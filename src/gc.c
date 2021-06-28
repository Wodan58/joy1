/*
    module  : gc.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef GC_C
#define GC_C

/**
3030  gc  :  ->
Initiates garbage collection.
*/
PRIVATE void gc_(pEnv env) { GC_gcollect(); }



#endif
