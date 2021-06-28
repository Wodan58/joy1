/*
    module  : %M%
    version : %I%
    date    : %G%
*/
#ifndef FREAD_C
#define FREAD_C

/**
1910  fread  :  S I  ->  S L
I bytes are read from the current position of stream S
and returned as a list of I integers.
*/
PRIVATE void fread_(pEnv env)
{
    FILE *fp;
    Index *my_dump;
    size_t i, count;
    unsigned char *buf;

    TWOPARAMS("fread");
    INTEGER("fread");
    count = nodevalue(env->stck).num;	// number of characters to read
    POP(env->stck);
    FILE("fread");
    fp = nodevalue(env->stck).fil;	// file descriptor
    buf = GC_malloc_atomic(count);	// buffer for characters to read
    NULLARY(LIST_NEWNODE, 0);		// list of integers
    my_dump = &nodevalue(env->stck).lis;
    count = fread(buf, 1, count, fp);	// number of characters read
    for (i = 0; i < count; i++) {
        *my_dump = INTEGER_NEWNODE(buf[i], 0);
        my_dump = &nextnode1(*my_dump);
    }
}
#endif
