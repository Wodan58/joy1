/*
    module  : gc.h
    version : 1.6
    date    : 06/14/20
*/
void GC_init(void *ptr);
void GC_gcollect(void);
void *GC_malloc_atomic(size_t size);
void *GC_malloc(size_t size);
void *GC_realloc(void *old, size_t size);
char *GC_strdup(const char *str);
