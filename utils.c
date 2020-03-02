/* FILE: utils.c */
/*
 *  module  : utils.c
 *  version : 1.15
 *  date    : 03/02/20
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#ifdef GC_BDW
#include <gc.h>
#endif

#ifndef GC_BDW
static Node
    memory[MEMORYMAX],
    *memoryindex = memory,
    *mem_low = memory,
    *mem_mid;
#define MEM_HIGH (MEMORYMAX-1)
static int direction = 1;
static int nodesinspected, nodescopied;
static int start_gc_clock;
#endif

PUBLIC void inimem1(void)
{
#ifdef SINGLE
    stk = NULL;
#else
    stk = conts = dump = dump1 = dump2 = dump3 = dump4 = dump5 = NULL;
#endif
#ifndef GC_BDW
    direction = 1;
    memoryindex = mem_low;
#endif
}

PUBLIC void inimem2(void)
{
#ifndef GC_BDW
    mem_low = memoryindex;
    mem_mid = mem_low + (&memory[MEM_HIGH] - mem_low) / 2;
    if (tracegc > 1) {
	printf("memory = %p : %p\n",
		(void *)memory, (void *)MEM2INT(memory));
	printf("memoryindex = %p : %p\n",
		(void *)memoryindex, (void *)MEM2INT(memoryindex));
	printf("mem_low = %p : %p\n",
		(void *)mem_low, (void *)MEM2INT(mem_low));
	printf("top of mem = %p : %p\n",
		(void *)&memory[MEM_HIGH],(void *)MEM2INT((&memory[MEM_HIGH])));
	printf("mem_mid = %p : %p\n",
		(void *)mem_mid, (void *)MEM2INT(mem_mid));
    }
#endif
}

#ifndef GC_BDW
PUBLIC void printnode(Node *p)
{
    printf("%10p:        %-10s %10p %10p\n",
	(void *)MEM2INT(p),
	symtab[(int) p->op].name,
	p->op == LIST_ ? (void *)MEM2INT(p->u.lis) : (void *)(size_t)p->u.num,
	(void *)MEM2INT(p->next));
}
#endif

#ifndef GC_BDW
PRIVATE Node *copy(Node *n)
{
    Node *temp;

    nodesinspected++;
    if (tracegc > 4)
	printf("copy ..\n");
    if (n == NULL)
	return NULL;
    if (n < mem_low)
	return n; /* later: combine with previous line */
    if (n->op == ILLEGAL_) {
	printf("copy: illegal node  ");
	printnode(n);
	return NULL;
    }
    if (n->op == COPIED_)
	return n->u.lis;
    temp = memoryindex;
    memoryindex += direction;
    temp->op = n->op;
/* Nick Forde recommmended replacing this line
    temp->u.num = n->op == LIST_ ? (long)copy(n->u.lis) : n->u.num;
   with the following case statement: */
    switch(n->op) {
    case INTEGER_:
	temp->u.num = n->u.num;
	break;
    case SET_:
	temp->u.set = n->u.set;
	break;
    case STRING_:
	temp->u.str = n->u.str;
	break;
    case FLOAT_:
	temp->u.dbl = n->u.dbl;
	break;
    case FILE_:
	temp->u.fil = n->u.fil;
	break;
    case LIST_:
	temp->u.lis = copy(n->u.lis);
	break;
    default:
	temp->u.num = n->u.num;
    }
/* end of replacement */
    temp->next = copy(n->next);
    n->op = COPIED_;
    n->u.lis = temp;
    nodescopied++;
    if (tracegc > 3) {
	printf("%5d -    ", nodescopied);
	printnode(temp);
    }
    return temp;
}
#endif

#ifndef GC_BDW
PRIVATE void gc1(char *mess)
{
    start_gc_clock = clock();
    if (tracegc > 1)
	printf("begin %s garbage collection\n", mess);
    direction = -direction;
    memoryindex = (direction == 1) ? mem_low : &memory[MEM_HIGH];
/*
    if (tracegc > 1) {
	printf("direction = %d\n", direction);
	printf("memoryindex = %d : %d\n",
		(long)memoryindex, MEM2INT(memoryindex));
    }
*/
    nodesinspected = nodescopied = 0;

#define COP(X, NAME)						\
    if (X != NULL) {						\
	if (tracegc > 2) {					\
	    printf("old %s = ", NAME);				\
	    writeterm(X, stdout); printf("\n"); }		\
	X = copy(X);						\
	if (tracegc > 2) {					\
	    printf("new %s = ", NAME);				\
	    writeterm(X, stdout); printf("\n"); } }

    COP(stk, "stk"); COP(prog, "prog"); COP(conts, "conts");
    COP(dump, "dump"); COP(dump1, "dump1"); COP(dump2, "dump2");
    COP(dump3, "dump3"); COP(dump4, "dump4"); COP(dump5, "dump5");
}

PRIVATE void gc2(char *mess)
{
    int this_gc_clock;

    this_gc_clock = clock() - start_gc_clock;
    if (this_gc_clock == 0)
	this_gc_clock = 1; /* correction */
    if (tracegc > 0)
	printf("gc - %d nodes inspected, %d nodes copied, clock: %d\n",
		nodesinspected, nodescopied, this_gc_clock);
    if (tracegc > 1)
	printf("end %s garbage collection\n", mess);
    gc_clock += this_gc_clock;
}
#endif

PUBLIC void gc_(void)
{
#ifndef GC_BDW
    gc1("user requested");
    gc2("user requested");
#else
   GC_gcollect();
#endif
}

#ifdef STATS
static double nodes;

static void report_nodes(void)
{
    fprintf(stderr, "%.0f nodes allocated\n", nodes);
}

static void count_nodes(void)
{
    if (++nodes == 1)
	atexit(report_nodes);
}
#endif

PUBLIC Node *newnode(Operator o, Types u, Node *r)
{
    Node *p;
#ifndef GC_BDW
    if (memoryindex == mem_mid) {
	gc1("automatic");
	if (o == LIST_)
	    u.lis = copy(u.lis);
	r = copy(r);
	gc2("automatic");
	if ((direction ==  1 && memoryindex >= mem_mid) ||
	    (direction == -1 && memoryindex <= mem_mid))
	    execerror("memory", "copying"); }
    p = memoryindex;
    memoryindex += direction;
#else
    if ((p = GC_malloc(sizeof(Node))) == 0)
	execerror("memory", "allocator");
#endif
    p->op = o;
    p->u = u;
    p->next = r;
#ifndef GC_BDW
D(  printnode(p); )
#endif
#ifdef STATS
    count_nodes();
#endif
    return p;
}

PUBLIC void memoryindex_(void)
{
#ifndef GC_BDW
    stk = INTEGER_NEWNODE(MEM2INT(memoryindex), stk);
#else
    stk = INTEGER_NEWNODE(0L, stk);
#endif
}

PUBLIC void readfactor(void)	/* read a JOY factor		*/
{
    long set = 0;

    switch (symb) {
    case ATOM:
	lookup();
D(  printf("readfactor: location = %p\n", (void *)location); )
	while (location->is_module) {
	    Entry *mod_fields = location->u.module_fields;
	    getsym();
	    if (symb != PERIOD)
		error("period '.' expected after module name");
	    else
		getsym();
	    if (symb != ATOM) {
		error("atom expected as module field");
		return;
	    }
D(  printf("looking for field %s\n", ident); )
	    while (mod_fields && strcmp(ident, mod_fields->name) != 0)
		mod_fields = mod_fields->next;
	    if (mod_fields == NULL) {
		error("no such field in module");
		abortexecution_();
	    }
D(  printf("found field: %s\n", mod_fields->name); )
	    location = mod_fields;
	}
/* end of replacement */
	if (location < firstlibra) {
	    bucket.proc = location->u.proc;
	    stk = newnode(LOC2INT(location), bucket, stk);
	} else
	    stk = USR_NEWNODE(location, stk);
	return;
    case BOOLEAN_:
    case INTEGER_:
    case CHAR_:
    case STRING_:
	bucket.num = numb;
	stk = newnode(symb, bucket, stk);
	return;
    case FLOAT_:
	stk = FLOAT_NEWNODE(dblf, stk);
	return;
    case LBRACE:
	while (getsym(), symb != RBRACE)
	    if (symb == CHAR_ || symb == INTEGER_)
		set |= ((long_t)1 << numb);
	    else
		error("numeric expected in set");
	stk = SET_NEWNODE(set, stk);
	return;
    case LBRACK:
	getsym();
	readterm();
	if (symb != RBRACK)
	    error("']' expected");
	return;
    case LPAREN:
	error("'(' not implemented");
	getsym();
	return;
    default:
	error("a factor cannot begin with this symbol");
    }
}

#ifndef SINGLE
#define SINGLE
#endif

PUBLIC void readterm(void)
{
#ifdef SINGLE
    Node **my_dump;
#endif
    stk = LIST_NEWNODE(0L, stk);
#ifdef SINGLE
    my_dump = &stk->u.lis;
#endif
    if (symb <= ATOM) {
	readfactor();
#ifdef SINGLE
	*my_dump = stk;
	my_dump = &stk->next;
	stk = stk->next;
#else
	stk->next->u.lis = stk;
	stk = stk->next;
	stk->u.lis->next = NULL;
	dump = newnode(LIST_, stk->u, dump);
#endif
	while (getsym(), symb <= ATOM) {
	    readfactor();
#ifdef SINGLE
	    *my_dump = stk;
	    my_dump = &stk->next;
	    stk = stk->next;
#else
	    dump->u.lis->next = stk;
	    stk = stk->next;
	    dump->u.lis->next->next = NULL;
	    dump->u.lis = dump->u.lis->next;
#endif
	}
#ifdef SINGLE
	*my_dump = 0;
#else
	dump = dump->next;
#endif
    }
}

PUBLIC void writefactor(Node *n, FILE *stm)
{
    char *p;
    int i;
    long set;

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
	writeterm(n->u.lis, stm);
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
	    fprintf(stm, "file:%p", (void *)n->u.fil);
	return;
    default:
	fprintf(stm, "%s", symtab[(int)n->op].name);
	return;
    }
}

PUBLIC void writeterm(Node *n, FILE *stm)
{
    while (n != NULL) {
	writefactor(n, stm);
	n = n->next;
	if (n != NULL)
	    fprintf(stm, " ");
    }
}
