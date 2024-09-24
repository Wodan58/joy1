/* FILE: globals.h */
/*
 *  module  : globals.h
 *  version : 1.113
 *  date    : 09/23/24
 */
#ifndef GLOBALS_H
#define GLOBALS_H

/* #define USE_KHASHL */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <setjmp.h>
#include <signal.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>

#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR) || defined(__TINYC__)
#define WINDOWS
#endif

#ifdef _MSC_VER
#define WINDOWS_S
#endif

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>		/* pollute name space as much as possible */
#include <io.h>			/* also import deprecated POSIX names */
#ifdef __TINYC__
#define strtoll _strtoi64	/* tcc 0.9.27 lacks strtoll */
#endif
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267 4996)
#define kh_packed		/* forget about __attribute__ ((packed)) */
#endif
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

#ifndef NOBDW
#ifdef _MSC_VER
#include "gc-8.2.8/include/gc.h"
#else
#include <gc.h>
#endif
#else
#include "gc.h"
#endif
#include "kvec.h"
#ifdef USE_KHASHL
#include "khashl.h"
#else
#include "khash.h"
#endif

#define POP(X) X = nextnode1(X)

#define USR_NEWNODE(u, r)						\
    (env->bucket.ent = u, newnode(env, USR_, env->bucket, r))
#define ANON_FUNCT_NEWNODE(u, r)					\
    (env->bucket.proc = u, newnode(env, ANON_FUNCT_, env->bucket, r))
#define BOOLEAN_NEWNODE(u, r)						\
    (env->bucket.num = u, newnode(env, BOOLEAN_, env->bucket, r))
#define CHAR_NEWNODE(u, r)						\
    (env->bucket.num = u, newnode(env, CHAR_, env->bucket, r))
#define INTEGER_NEWNODE(u, r)						\
    (env->bucket.num = u, newnode(env, INTEGER_, env->bucket, r))
#define SET_NEWNODE(u, r)						\
    (env->bucket.set = u, newnode(env, SET_, env->bucket, r))
#define STRING_NEWNODE(u, r)						\
    (env->bucket.str = u, newnode(env, STRING_, env->bucket, r))
#define LIST_NEWNODE(u, r)						\
    (env->bucket.lis = u, newnode(env, LIST_, env->bucket, r))
#define FLOAT_NEWNODE(u, r)						\
    (env->bucket.dbl = u, newnode(env, FLOAT_, env->bucket, r))
#define FILE_NEWNODE(u, r)						\
    (env->bucket.fil = u, newnode(env, FILE_, env->bucket, r))
#define BIGNUM_NEWNODE(u, r)						\
    (env->bucket.str = u, newnode(env, BIGNUM_, env->bucket, r))

#define NULLARY(CONSTRUCTOR, VALUE)					\
    env->stck = CONSTRUCTOR(VALUE, env->stck)
#define UNARY(CONSTRUCTOR, VALUE)					\
    env->stck = CONSTRUCTOR(VALUE, nextnode1(env->stck))
#define BINARY(CONSTRUCTOR, VALUE)					\
    env->stck = CONSTRUCTOR(VALUE, nextnode2(env->stck))

#ifdef NOBDW
#define nodetype(n)  env->memory[n].op
#define nodeleng(n)  env->memory[n].len
#define nodevalue(n) env->memory[n].u
#define nextnode1(n) env->memory[n].next
#define nextnode2(n) env->memory[nextnode1(n)].next
#define nextnode3(n) env->memory[nextnode2(n)].next
#define nextnode4(n) env->memory[nextnode3(n)].next
#define nextnode5(n) env->memory[nextnode4(n)].next
#else
#define nodetype(p)  (p)->op
#define nodevalue(p) (p)->u
#define nextnode1(p) (p)->next
#define nextnode2(p) (nextnode1(p))->next
#define nextnode3(p) (nextnode2(p))->next
#define nextnode4(p) (nextnode3(p))->next
#define nextnode5(p) (nextnode4(p))->next
#ifdef TRACEGC
#undef TRACEGC
#endif
#endif

/* configure			*/
#define SHELLESCAPE	'$'
#define INPSTACKMAX	10
#define INPLINEMAX	255
#define BUFFERMAX	80	/* smaller buffer */
#define HELPLINEMAX	72
#define MAXNUM		40	/* even smaller buffer */
#define FILENAMEMAX	14
#define DISPLAYMAX	10	/* nesting in HIDE & MODULE */
#define INIECHOFLAG	0
#define INIAUTOPUT	1
#define INITRACEGC	1
#define INIUNDEFERROR	0
#define INIWARNING	1

/* installation dependent	*/
#define SETSIZE (int)(CHAR_BIT * sizeof(uint64_t))	/* from limits.h */
#define MAXINT_ INT64_MAX				/* from stdint.h */

/* symbols from getsym		*/
enum {
    ILLEGAL_,
    COPIED_,
    USR_,
    ANON_FUNCT_,
    BOOLEAN_,
    CHAR_,
    INTEGER_,
    SET_,
    STRING_,
    LIST_,
    FLOAT_,
    FILE_,
    BIGNUM_,

    LIBRA,
    EQDEF,
    HIDE,
    IN__,
    MODULE_,
    PRIVATE,
    PUBLIC,
    CONST_
};

typedef enum {
    OK,
    IGNORE_OK,
    IGNORE_PUSH,
    IGNORE_POP,
    IMMEDIATE,
    POSTPONE
} Flags;

typedef enum {
    ABORT_NONE,
    ABORT_RETRY,
    ABORT_QUIT
} Abort;

/* types			*/
typedef unsigned char Operator;	/* opcode / datatype */

typedef struct Env *pEnv;

typedef void (*proc_t)(pEnv);	/* procedure */

#ifdef NOBDW
typedef unsigned Index;
#else
typedef struct Node *Index;
#endif

typedef union {
    int64_t num;	/* USR, BOOLEAN, CHAR, INTEGER */
    proc_t proc;	/* ANON_FUNCT */
    uint64_t set;	/* SET */
    char *str;		/* STRING */
    Index lis;		/* LIST */
    double dbl;		/* FLOAT */
    FILE *fil;		/* FILE */
    int ent;		/* SYMBOL */
} Types;

#ifdef NOBDW
typedef struct Node {
    unsigned op :  4,
	    len : 28;	/* length of string */
    Index next;
    Types u;
} Node;
#else
typedef struct Node {
    Operator op;
    Index next;
    Types u;
} Node;
#endif

typedef struct Token {
    Operator op;
    int x, y, pos;
    Types u;
} Token;

typedef struct Entry {
    char *name;
    unsigned char is_user, flags, is_ok, is_root, is_last;
    union {
	Index body;
	proc_t proc;
    } u;
} Entry;

#ifdef USE_KHASHL
KHASHL_MAP_INIT(KH_LOCAL, symtab_t, symtab, const char *, int, kh_hash_str,
		kh_eq_str)
KHASHL_MAP_INIT(KH_LOCAL, funtab_t, funtab, uint64_t, int, kh_hash_uint64,
		kh_eq_generic)
#else
KHASH_MAP_INIT_STR(Symtab, int)
KHASH_MAP_INIT_INT64(Funtab, int)
#endif

typedef struct Env {
    jmp_buf finclude;		/* return point in finclude */
    double nodes;		/* statistics */
    double avail;
    double collect;
    double calls;
    double opers;
    double dbl;			/* numerics */
    int64_t num;
    char *str;			/* string */
    clock_t startclock;		/* main */
    char **g_argv;		/* command line */
    char *filename;		/* first include file */
    char *homedir;		/* HOME or HOMEPATH */
    char *mod_name;		/* name of module */
    vector(char *) *pathnames;	/* pathnames to be searched when including */
    vector(char) *string;	/* value */
    vector(char) *pushback;	/* push back buffer */
    vector(Token) *tokens;	/* read ahead table */
    vector(Entry) *symtab;	/* symbol table */
#ifdef USE_KHASHL
    symtab_t *hash;		/* hash tables that index the symbol table */
    funtab_t *prim;
#else
    khash_t(Symtab) *hash;
    khash_t(Funtab) *prim;
#endif
    Types bucket;		/* used by NEWNODE defines */
#ifdef NOBDW
    clock_t gc_clock;
    Node *memory;		/* dynamic memory */
    Index conts, dump, dump1, dump2, dump3, dump4, dump5, inits;
#endif
    Index prog, stck;
    int g_argc;			/* command line */
    int hide_stack[DISPLAYMAX];
    struct {
	char *name;
	int hide;
    } module_stack[DISPLAYMAX];
    Operator sym;		/* symbol */
    unsigned char inlining;
    unsigned char autoput;
    unsigned char autoput_set;
    unsigned char echoflag;
    unsigned char tracegc;
    unsigned char undeferror;
    unsigned char undeferror_set;
    unsigned char bytecoding;	/* BDW only */
    unsigned char compiling;	/* BDW only */
    unsigned char debugging;
    unsigned char ignore;
    unsigned char overwrite;
    unsigned char printing;
    unsigned char finclude_busy;
    unsigned char flibrary_busy;
    unsigned char variable_busy;
} Env;

typedef struct table_t {
    proc_t proc;
    char *name;
} table_t;

/* GOOD REFS:
	005.133l H4732		A LISP interpreter in C
	Manna p139  recursive Ackermann SCHEMA

   OTHER DATA TYPES
	WORD = "ABCD" - up to four chars
	LIST of SETs of char [S0 S1 S2 S3]
		LISTS - binary tree [left right]
			" with info [info left right] "
	STRING of 32 chars = 32 * 8 bits = 256 bits = bigset
	CHAR = 2 HEX
	32 SET = 2 * 16SET
*/

/* Public procedures: */
/* main.c */
void abortexecution_(int num);
void fatal(char *str);
/* interp.c */
void exeterm(pEnv env, Index n);
/* scan.c */
void inilinebuffer(pEnv env);
int getch(pEnv env);
void ungetch(int ch);
void error(char *str);
int include(pEnv env, char *name);
int getsym(pEnv env, int ch);
/* utils.c */
Index newnode(pEnv env, Operator o, Types u, Index r);
Index newnode2(pEnv env, Index n, Index r);
void my_memoryindex(pEnv env);
void my_memorymax(pEnv env);
#ifdef NOBDW
void inimem1(pEnv env, int status);
void inimem2(pEnv env);
void printnode(pEnv env, Index p);
void my_gc(pEnv env);
#endif
/* error.c */
void execerror(char *message, char *op);
/* factor.c */
int readfactor(pEnv env, int ch, int *rv);	/* read a JOY factor */
int readterm(pEnv env, int ch);
/* module.c */
void savemod(int *hide, int *modl, int *hcnt);
void undomod(int hide, int modl, int hcnt);
void initmod(pEnv env, char *name);
void initpriv(pEnv env);
void stoppriv(void);
void exitpriv(void);
void exitmod(void);
char *classify(pEnv env, char *name);
int qualify(pEnv env, char *name);
/* optable.c */
#ifdef BYTECODE
int tablesize(void);
int operqcode(int index);
#endif
char *nickname(int ch);
char *opername(int o);
int operindex(pEnv env, proc_t proc);
void inisymboltable(pEnv env);			/* initialise */
void addsymbol(pEnv env, Entry ent, int index);
/* print.c */
void print(pEnv env);
/* repl.c */
void repl(pEnv env);
/* setraw.c */
void SetRaw(pEnv env);
/* symbol.c */
int lookup(pEnv env, char *name);
int enteratom(pEnv env, char *name);
int compound_def(pEnv env, int ch);
/* undefs.c */
void hide_inner_modules(pEnv env, int flag);
/* write.c */
void writefactor(pEnv env, Index n, FILE *fp);
void writeterm(pEnv env, Index n, FILE *fp);
#ifdef BYTECODE
/* bytecode.c */
void bytecode(pEnv env, Node *list);
void initbytes(pEnv env);
void exitbytes(pEnv env);
/* compile.c */
int compile(pEnv env, Node *node);
void initcompile(pEnv env);
void exitcompile(pEnv env);
/* dumpbyte.c */
void dumpbyte(pEnv env, FILE *fp);
/* readbyte.c */
void readbyte(pEnv env, FILE *fp, int flag);
unsigned char *readfile(FILE *fp);
/* optimize.c */
void optimize(pEnv env, FILE *fp);
#endif
#endif
