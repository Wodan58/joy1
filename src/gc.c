/*
    module  : gc.c
    version : 1.2
    date    : 08/13/23
*/
#ifndef GC_C
#define GC_C

/**
OK 3030  gc  :  ->
Initiates garbage collection.
*/
PRIVATE void gc_(pEnv env) { GC_gcollect(); }



#endif
