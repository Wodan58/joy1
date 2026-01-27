/*
 *  module  : readbyte.c
 *  version : 1.6
 *  date    : 01/22/26
 */
#include "globals.h"

static int readterm2(pEnv env, int ch, FILE *fp);

/*
 * entersub - enter a subroutine in the symbol table.
 */
static void entersub(pEnv env, Node *list, int leng)
{
    Entry ent;
    char buf[MAXNUM];

    sprintf(buf, "%c%d", list ? '#' : '_', leng);	/* _ = variables */
    memset(&ent, 0, sizeof(ent));
    ent.name = strdup(buf);	/* not garbage collected */
    ent.is_user = 1;
    ent.u.body = list;
    vec_push(env->symtab, ent);
}

/*
 * readfactor2 - read a factor from fp and push it on the stack.
 */
static int readfactor2(pEnv env, int ch, FILE *fp)	/* read a JOY factor */
{
    static int offset;
    int i;
    Node node;
    Entry ent;
    char buf[BUFFERMAX];	/* assume strings are no longer than this */

    if (!offset)
	offset = tablesize() - 1;
    node.op = ch;
    ch = fgetc(fp);
    switch (node.op) {
    case 0:			/* should not happen */
	break;

    case USR_:
	for (i = 0; ch > 1; i++) {
	    buf[i] = ch;
	    ch = fgetc(fp);
	}
	buf[i] = 0;
	node.u.ent = atoi(buf) + offset;	/* offset in table */
	GNULLARY(&node);
	ch = fgetc(fp);
	break;

    case BOOLEAN_:
	node.u.num = ch == '1';
	GNULLARY(&node);
	ch = fgetc(fp);
	break;

    case CHAR_:
	node.u.num = ch;
	GNULLARY(&node);
	ch = fgetc(fp);
	break;

    case INTEGER_:
    case SET_:
	for (i = 0; ch > 1; i++) {
	    buf[i] = ch;
	    ch = fgetc(fp);
	}
	buf[i] = 0;
	node.u.num = strtoll(buf, 0, 0);
	GNULLARY(&node);
	ch = fgetc(fp);
	break;

    case STRING_:
    case BIGNUM_:
	for (i = 0; ch > 1; i++) {
	    buf[i] = ch;
	    ch = fgetc(fp);
	}
	buf[i] = 0;
	node.u.str = GC_strdup(buf);
	GNULLARY(&node);
	ch = fgetc(fp);
	break;

    case LIST_:
	ch = readterm2(env, ch, fp);
	break;

    case FLOAT_:
	for (i = 0; ch > 1; i++) {
	    buf[i] = ch;
	    ch = fgetc(fp);
	}
	buf[i] = 0;
	node.u.dbl = strtod(buf, 0);
	GNULLARY(&node);
	ch = fgetc(fp);
	break;

    default:
	ent = vec_at(env->symtab, node.op);
	/* evaluate immediate functions at compile time */
	if (ent.flags == IMMEDIATE) {
	    if (ent.is_user)
		exeterm(env, ent.u.body);
	    else
		(*ent.u.proc)(env);
	} else {
	    node.u.proc = ent.u.proc;
	    node.op = ANON_FUNCT_;
	    GNULLARY(&node);
	}
	break;
    }
    return ch;
}

/*
 * readterm2 - read a term from fp and push this on the stack as a list.
 */
static int readterm2(pEnv env, int ch, FILE *fp)
{
    Node **dump;

    NULLARY(LIST_NEWNODE, 0);
    dump = &env->stck->u.lis;
    while (ch > 1) {			/* 1 is used instead of ] */
	ch = readfactor2(env, ch, fp);
	*dump = env->stck;
	dump = &env->stck->next;
	env->stck = *dump;
	*dump = 0;
    }
    return fgetc(fp);
}

/*
 * readbyte - store the main program in env->prog and add subroutines to the
 *	      symbol table.
 */
void readbyte(pEnv env, FILE *fp)
{
    int i, ch;
    Node *node;

    ch = readterm2(env, fgetc(fp), fp);	/* read main program */
    env->prog = env->stck->u.lis;
    POP(env->stck);
    for (i = 1; ch > 0; i++) {		/* subroutine parsing */
	ch = readterm2(env, ch, fp);
	node = env->stck->u.lis;
	POP(env->stck);
	entersub(env, node, i);
    }
}
