/* FILE: factor.c */
/*
 *  module  : factor.c
 *  version : 1.8
 *  date    : 05/02/22
 */
#include "globals.h"

/*
    readfactor - read a factor from srcfile and push it on the stack.
                 In case of LPAREN nothing gets pushed.
*/
PUBLIC void readfactor(pEnv env, int priv) /* read a JOY factor */
{
    Entry ent;
    long_t set = 0;

    switch (env->symb) {
    case ATOM:
        if (!priv) {
            lookup(env);
            if (!env->location && strchr(env->ident, '.')) {
                error(env, "no such field in module");
                return;
            }
            ent = vec_at(env->symtab, env->location);
            if (!ent.is_user) {
                env->yylval.proc = ent.u.proc;
                env->stck = newnode(env, env->location, env->yylval, env->stck);
            } else {
                env->bucket.ent = env->location;
                env->stck = newnode(env, USR_, env->bucket, env->stck);
            }
        }
        return;
    case BOOLEAN_:
    case CHAR_:
    case INTEGER_:
    case STRING_:
    case FLOAT_:
        if (!priv)
            env->stck = newnode(env, env->symb, env->yylval, env->stck);
        return;
    case LBRACE:
        while (getsym(env), env->symb <= ATOM)
            if ((env->symb != CHAR_ && env->symb != INTEGER_)
                || env->yylval.num < 0 || env->yylval.num >= SETSIZE)
                error(env, "small numeric expected in set");
            else
                set |= ((long_t)1 << env->yylval.num);
        if (!priv) {
            env->bucket.set = set;
            env->stck = newnode(env, SET_, env->bucket, env->stck);
        }
        if (env->symb != RBRACE)
            error(env, "'}' expected");
        return;
    case LBRACK:
        getsym(env);
        readterm(env, priv);
        if (env->symb != RBRACK)
            error(env, "']' expected");
        return;
    case LPAREN:
        error(env, "'(' not implemented");
        getsym(env);
        return;
    default:
        error(env, "a factor cannot begin with this symbol");
    }
}

/*
    readterm - read a term from srcfile and push this on the stack as a list.
*/
PUBLIC void readterm(pEnv env, int priv)
{
    Index *dump = 0;

    if (!priv) {
        env->bucket.lis = 0;
        env->stck = newnode(env, LIST_, env->bucket, env->stck);
        dump = &nodevalue(env->stck).lis;
    }
    while (env->symb <= ATOM) {
        readfactor(env, priv);
        if (!priv && env->stck) {
            *dump = env->stck;
            dump = &nextnode1(env->stck);
            env->stck = *dump;
            *dump = 0;
        }
        getsym(env);
    }
}

/*
    writefactor - print a factor in readable format to stdout.
*/
PUBLIC void writefactor(pEnv env, Index n)
{
    int i;
    char *p;
    long_t set;

/*
    This cannot happen. Factor has a small number of customers: writeterm,
    main, put, fput. They all check that the stack is not empty, so this code
    only serves as a reminder for future customers.
*/
#if 0
    if (!n)
        execerror("non-empty stack", "print");
#endif
    switch (opertype(nodetype(n))) {
    case USR_:
        printf("%s", vec_at(env->symtab, nodevalue(n).ent).name);
        return;
    case BOOLEAN_:
        printf("%s", nodevalue(n).num ? "true" : "false");
        return;
    case CHAR_:
        printf("'%c", (char)nodevalue(n).num);
        return;
    case INTEGER_:
        printf("%ld", (long)nodevalue(n).num); /* BIT_32 */
        return;
    case SET_:
        putchar('{');
        for (i = 0, set = nodevalue(n).set; i < SETSIZE; i++)
            if (set & ((long_t)1 << i)) {
                printf("%d", i);
                set &= ~((long_t)1 << i);
                if (set)
                    putchar(' ');
            }
        putchar('}');
        return;
    case STRING_:
        putchar('"');
        for (p = nodevalue(n).str; *p; p++) {
            if (*p == '"' || *p == '\\' || *p == '\n')
                putchar('\\');
            if (*p == '\n')
                putchar('n');
            else
                putchar(*p);
        }
        putchar('"');
        return;
    case LIST_:
        putchar('[');
        writeterm(env, nodevalue(n).lis);
        putchar(']');
        return;
    case FLOAT_:
        printf("%g", nodevalue(n).dbl);
        return;
    case FILE_:
        if (!nodevalue(n).fil)
            printf("file:NULL");
        else if (nodevalue(n).fil == stdin)
            printf("file:stdin");
        else if (nodevalue(n).fil == stdout)
            printf("file:stdout");
        else if (nodevalue(n).fil == stderr)
            printf("file:stderr");
        else
            printf("file:%p", nodevalue(n).fil);
        return;
    default:
        printf("%s", opername(nodetype(n)));
        return;
    }
}

/*
    writeterm - print the contents of a list in readable format to stdout.
*/
PUBLIC void writeterm(pEnv env, Index n)
{
    while (n) {
        writefactor(env, n);
        if ((n = nextnode1(n)) != 0)
            putchar(' ');
    }
}