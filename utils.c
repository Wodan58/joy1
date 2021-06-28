/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.29
 *  date    : 06/28/21
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gc.h"
#include "globals.h"

#ifdef STATS
static double nodes;

static void report_nodes(void) { fprintf(stderr, "%.0f nodes used\n", nodes); }

static void count_nodes(void)
{
    if (++nodes == 1)
        atexit(report_nodes);
}
#endif

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

PUBLIC void readterm(pEnv env, int priv)
{
    Node **my_dump;

    if (!priv) {
        env->stck = LIST_NEWNODE(0, env->stck);
        my_dump = &nodevalue(env->stck).lis;
    }
    while (symb <= ATOM) {
        readfactor(env, priv);
        if (!priv) {
            *my_dump = env->stck;
            my_dump = &nextnode1(env->stck);
            env->stck = *my_dump;
            *my_dump = 0;
        }
        getsym(env);
    }
}

PUBLIC void writefactor(pEnv env, Node *n, FILE *stm)
{
    my_writefactor(env, n, stm);
}

PUBLIC void writeterm(pEnv env, Node *n, FILE *stm)
{
    while (n != NULL) {
        writefactor(env, n, stm);
        n = n->next;
        if (n != NULL)
            fprintf(stm, " ");
    }
}
