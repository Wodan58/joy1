/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.50
 *  date    : 10/11/24
 */
#include "globals.h"

/*
 * newnode - allocate a new node or die trying.
 */
Index newnode(pEnv env, Operator o, Types u, Index r)
{
    Index p;

    p = GC_malloc(sizeof(Node));
#ifdef _MSC_VER
    if (!p)
	execerror(env, "memory", "allocator");
#endif
    p->op = o;
    p->u = u;
    p->next = r;
    env->nodes++;
    return p;
}

/*
 * newnode2 - allocate a node as copy of an existing node.
 */
Index newnode2(pEnv env, Index n, Index r)
{
    return newnode(env, n->op, n->u, r);
}

/*
 * Report the number of nodes that are currently being used.
 */
void my_memoryindex(pEnv env)
{
    NULLARY(INTEGER_NEWNODE, GC_get_memory_use());
}

/*
 * Report the total number of nodes that are currently available.
 */
void my_memorymax(pEnv env)
{
    NULLARY(INTEGER_NEWNODE, GC_get_memory_use() + GC_get_free_bytes());
}
