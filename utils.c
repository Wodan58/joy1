/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.23
 *  date    : 06/25/20
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <gc.h>
#include "globals.h"

PUBLIC void gc_(pEnv env) { GC_gcollect(); }

#ifdef STATS
static double nodes;

static void report_nodes(void)
{
    fprintf(stderr, "%.0f nodes used\n", nodes);
}

static void count_nodes(void)
{
    if (++nodes == 1)
        atexit(report_nodes);
}
#endif

PUBLIC Node* newnode(Operator o, Types u, Node* r)
{
    Node* p;

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

PUBLIC void memoryindex_(pEnv env)
{
    env->stck = INTEGER_NEWNODE(0, env->stck);
}

PUBLIC void readfactor(pEnv env, int priv) /* read a JOY factor	*/
{
    long_t set = 0;

    switch (symb) {
    case ATOM:
        lookup(env, priv);
        while (location && location->is_module) {
            Entry* mod_fields = location->u.module_fields;
            getsym(env);
            if (symb != PERIOD)
                error("period '.' expected after module name");
            else
                getsym(env);
            if (symb != ATOM) {
                error("atom expected as module field");
                return;
            }
            while (mod_fields && strcmp(ident, mod_fields->name) != 0)
                mod_fields = mod_fields->next;
            if (mod_fields == NULL) {
                error("no such field in module");
                abortexecution_(env);
            }
            location = mod_fields;
        }
        if (!priv) {
            if (location < firstlibra) {
                env->bucket.proc = location->u.proc;
                env->stck = newnode(LOC2INT(location), env->bucket, env->stck);
            } else
                env->stck = USR_NEWNODE(location, env->stck);
        }
        return;
    case BOOLEAN_:
    case INTEGER_:
    case CHAR_:
        if (!priv)
            env->stck = newnode(symb, env->bucket, env->stck);
        return;
    case STRING_:
        if (!priv)
            env->stck = newnode(symb, env->bucket, env->stck);
        return;
    case FLOAT_:
        if (!priv)
            env->stck = FLOAT_NEWNODE(env->bucket.dbl, env->stck);
        return;
    case LBRACE:
        while (getsym(env), symb != RBRACE)
            if (symb == CHAR_ || symb == INTEGER_)
                set |= ((long_t)1 << env->bucket.num);
            else
                error("numeric expected in set");
        if (!priv)
            env->stck = SET_NEWNODE(set, env->stck);
        return;
    case LBRACK:
        getsym(env);
        readterm(env, priv);
        if (symb != RBRACK)
            error("']' expected");
        return;
    case LPAREN:
        error("'(' not implemented");
        getsym(env);
        return;
    default:
        error("a factor cannot begin with this symbol");
    }
}

PUBLIC void readterm(pEnv env, int priv)
{
    Node** dump;

    if (!priv) {
        env->stck = LIST_NEWNODE(0, env->stck);
        dump = &env->stck->u.lis;
    }
    while (symb <= ATOM) {
        readfactor(env, priv);
        if (!priv) {
            *dump = env->stck;
            dump = &env->stck->next;
            env->stck = *dump;
            *dump = 0;
        }
        getsym(env);
    }
}

PUBLIC void writefactor(pEnv env, Node* n, FILE* stm)
{
    char* p;
    int i;
    long_t set;

    if (n == NULL)
        execerror("non-empty stack", "print");
    switch (n->op) {
    case BOOLEAN_:
        fprintf(stm, "%s", n->u.num ? "true" : "false");
        return;
    case INTEGER_:
#ifdef BIT_32
        fprintf(stm, "%ld", (long)n->u.num);
#else
        fprintf(stm, "%lld", n->u.num);
#endif
        return;
    case FLOAT_:
        fprintf(stm, "%g", n->u.dbl);
        return;
    case SET_:
        set = n->u.set;
        fprintf(stm, "{");
        for (i = 0; i < SETSIZE; i++)
            if (set & ((long_t)1 << i)) {
                fprintf(stm, "%d", i);
                set &= ~((long_t)1 << i);
                if (set != 0)
                    fprintf(stm, " ");
            }
        fprintf(stm, "}");
        return;
    case CHAR_:
        fprintf(stm, "'%c", (char)n->u.num);
        return;
    case STRING_:
        fputc('"', stm);
        for (p = n->u.str; p && *p; p++) {
            if (*p == '"' || *p == '\\' || *p == '\n')
                fputc('\\', stm);
            fputc(*p == '\n' ? 'n' : *p, stm);
        }
        fputc('"', stm);
        return;
    case LIST_:
        fprintf(stm, "%s", "[");
        writeterm(env, n->u.lis, stm);
        fprintf(stm, "%s", "]");
        return;
    case USR_:
        fprintf(stm, "%s", n->u.ent->name);
        return;
    case FILE_:
        if (n->u.fil == NULL)
            fprintf(stm, "file:NULL");
        else if (n->u.fil == stdin)
            fprintf(stm, "file:stdin");
        else if (n->u.fil == stdout)
            fprintf(stm, "file:stdout");
        else if (n->u.fil == stderr)
            fprintf(stm, "file:stderr");
        else
            fprintf(stm, "file:%p", (void*)n->u.fil);
        return;
    default:
        fprintf(stm, "%s", opername(n->op));
        return;
    }
}

PUBLIC void writeterm(pEnv env, Node* n, FILE* stm)
{
    while (n != NULL) {
        writefactor(env, n, stm);
        n = n->next;
        if (n != NULL)
            fprintf(stm, " ");
    }
}
