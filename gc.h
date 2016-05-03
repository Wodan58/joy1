void *GC_malloc_atomic(size_t size);
void *GC_realloc(void *old_ptr, size_t new_size);
void GC_gcollect(void);
void *GC_malloc(size_t size);
void GC_init(void);
