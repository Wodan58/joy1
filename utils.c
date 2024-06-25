/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.46
 *  date    : 06/21/24
 */
#include "globals.h"

/*
 * newnode - allocate a new node or die trying.
 */
Index newnode(pEnv env, Operator o, Types u, Index r)
{
    Node *p;

    if ((p = GC_malloc(sizeof(Node))) == 0)
	execerror("memory", "allocator");
    p->op = o;
    p->u = u;
    p->next = r;
    env->nodes++;
    return p;
}

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
