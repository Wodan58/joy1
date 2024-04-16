/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.45
 *  date    : 04/09/24
 */
#include "globals.h"

/*
 * newnode - allocate a new node or die trying. If there is a maximum number of
 *           nodes, an error will be generated if more nodes are requested than
 *           the maximum.
 */
Index newnode(pEnv env, Operator o, Types u, Index r)
{
    Node *p;

    if (env->maxnodes && env->nodes >= env->maxnodes)
	fatal("memory excedes maxnodes");
    if ((p = GC_malloc(sizeof(Node))) == 0)
	execerror("memory", "allocator");
    p->op = o;
    p->u = u;
    p->next = r;
    env->nodes++;
    return p;
}

void my_memoryindex(pEnv env)
{
    env->bucket.num = GC_get_memory_use();
    env->stck = newnode(env, INTEGER_, env->bucket, env->stck);
}

void my_memorymax(pEnv env)
{
    env->bucket.num = GC_get_memory_use() + GC_get_free_bytes();
    env->stck = newnode(env, INTEGER_, env->bucket, env->stck);
}
