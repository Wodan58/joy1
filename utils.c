/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.43
 *  date    : 02/12/24
 */
#include "globals.h"

#ifdef STATS
static double nodes;

PRIVATE void report_nodes(pEnv env)
{
    if (!env->statistics)
	return;
    fflush(stdout);
    fprintf(stderr, "%.0f nodes used\n", nodes);
    fprintf(stderr, "%.0f garbage collections\n", (double)GC_get_gc_no());
}

PRIVATE void count_nodes(void)
{
    if (++nodes == 1)
	my_atexit(report_nodes);
}
#endif

/*
    newnode - allocate a new node or die trying.
*/
PUBLIC Node *newnode(pEnv env, Operator o, Types u, Node *r)
{
    Node *p;

    if ((p = GC_malloc(sizeof(Node))) == 0)
	execerror("memory", "allocator");
    p->op = o;
    p->u = u;
    p->next = r;
#ifdef STATS
    count_nodes();
#endif
    return p;
}

PUBLIC void my_memoryindex(pEnv env)
{
    if (env->ignore)
	env->bucket.num = 0;
    else
	env->bucket.num = GC_get_memory_use();
    env->stck = newnode(env, INTEGER_, env->bucket, env->stck);
}

PUBLIC void my_memorymax(pEnv env)
{
    if (env->ignore)
	env->bucket.num = 0;
    else
	env->bucket.num = GC_get_memory_use() + GC_get_free_bytes();
    env->stck = newnode(env, INTEGER_, env->bucket, env->stck);
}
