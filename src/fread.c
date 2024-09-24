/*
    module  : fread.c
    version : 1.11
    date    : 09/17/24
*/
#ifndef FREAD_C
#define FREAD_C

/**
Q0  OK  1900  fread  :  S I  ->  S L
[FOREIGN] I bytes are read from the current position of stream S
and returned as a list of I integers.
*/
void fread_(pEnv env)
{
    FILE *fp;
    int i, count;
    Index list = 0;
    unsigned char *volatile buf;

    TWOPARAMS("fread");
    INTEGER("fread");
    count = nodevalue(env->stck).num;	// number of characters to read
    POP(env->stck);
    ISFILE("fread");
    fp = nodevalue(env->stck).fil;	// file descriptor
    buf = GC_malloc_atomic(count);	// buffer for characters to read
    count = fread(buf, 1, count, fp);	// number of characters read
    for (i = count - 1; i >= 0; i--)
	list = INTEGER_NEWNODE(buf[i], list);
    NULLARY(LIST_NEWNODE, list);
}
#endif
