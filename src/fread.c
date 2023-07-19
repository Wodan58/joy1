/*
    module  : fread.c
    version : 1.5
    date    : 07/19/23
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
    Index list = 0;
    int64_t i, count;
    unsigned char *volatile buf;

    TWOPARAMS("fread");
    INTEGER("fread");
    count = nodevalue(env->stck).num;	// number of characters to read
    POP(env->stck);
    FILE("fread");
    fp = nodevalue(env->stck).fil;	// file descriptor
    buf = GC_malloc_atomic(count);	// buffer for characters to read
    count = fread(buf, 1, count, fp);	// number of characters read
    for (i = count - 1; i >= 0; i--)
	list = INTEGER_NEWNODE(buf[i], list);
    NULLARY(LIST_NEWNODE, list);
}
#endif
