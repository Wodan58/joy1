/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.31
 *  date    : 04/12/22
 */
#include "globals.h"

#ifdef STATS
static double nodes;

PRIVATE void report_nodes(void) { fprintf(stderr, "%.0f nodes used\n", nodes); }

PRIVATE void count_nodes(void)
{
    if (++nodes == 1)
        atexit(report_nodes);
}
#endif

/*
    newnode - allocate a new node or die trying.
*/
PUBLIC Node *newnode(pEnv env, Operator o, Types u, Node *r)
{
    Node *p;

    if ((p = GC_malloc(sizeof(Node))) == 0)
        execerror(env, "memory", "allocator");
    p->op = o;
    p->u = u;
    p->next = r;
#ifdef STATS
    count_nodes();
#endif
    return p;
}
