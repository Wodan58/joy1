/*
    module  : scan.c
    version : 1.68
    date    : 05/02/24
*/
#include "globals.h"

static struct keys {
    char *name;
    Operator sym;
} keywords[] = {
    { "LIBRA",   LIBRA   },
    { "DEFINE",  LIBRA   },
    { "HIDE",    HIDE    },
    { "IN",      IN__    },
    { "END",     '.'     },
    { "MODULE",  MODULE_ },
    { "PRIVATE", PRIVATE },
    { "PUBLIC",  PUBLIC  },
    { "CONST",   CONST_  },
    { "INLINE",  CONST_  },
    /* possibly other uppers here */
    { "==",      EQDEF   }
};

static FILE *srcfile;			/* FILE pointer to input file */
static char *filenam;			/* name of input file in messages */
static int linenum, linepos;		/* line number for errors */
static char linebuf[INPLINEMAX + 1];	/* buffered input line */
static struct {
    FILE *fp;
    int line;
    char name[FILENAMEMAX + 1];		/* filename in error messages */
} infile[INPSTACKMAX];
static int ilevel, startpos;		/* index in infile-structure */

/*
 * inilinebuffer - initialise the stack of input files. The filename parameter
 *		   is used in error messages.
 */
void inilinebuffer(FILE *fp, char *str)
{
    infile[0].fp = srcfile = fp;
    infile[0].line = linenum = 1;
    strncpy(infile[0].name, str, FILENAMEMAX);
    filenam = infile[0].name;
}

/*
 * getch reads the next character from srcfile.
 */
int getch(pEnv env)
{
    int ch;

again:
    if (vec_size(env->pushback))
	return vec_pop(env->pushback);
    if ((ch = fgetc(srcfile)) == EOF) {
	if (!ilevel)
	    abortexecution_(ABORT_QUIT);
	fclose(srcfile);
	if (env->finclude_busy)
	    longjmp(env->finclude, 1);	/* back to finclude */
	srcfile = infile[--ilevel].fp;
	linenum = infile[ilevel].line;
	filenam = infile[ilevel].name;
	goto again;
    }
    if (!linepos && ch == SHELLESCAPE) {
	vec_setsize(env->string, 0);
	while ((ch = fgetc(srcfile)) != '\n' && ch != EOF)
	    vec_push(env->string, ch);
	vec_push(env->string, 0);
	if (!env->ignore)
	    if ((ch = system(&vec_at(env->string, 0))) != 0) {
		fflush(stdout);
		fprintf(stderr, "system: %d\n", ch);
	    }
	vec_setsize(env->string, 0);
	goto again;
    }
    if (ch == '\n') {
	if (env->echoflag > 2)
	    printf("%4d", linenum);
	if (env->echoflag > 1)
	    putchar('\t');
	if (env->echoflag)
	    printf("%.*s\n", linepos, linebuf);	/* echo line */
	linenum++;
	linepos = 0;
    } else if (linepos < INPLINEMAX) {
	linebuf[linepos++] = ch;
	linebuf[linepos] = 0;
    }
    return ch;
}

/*
 * ungetch unreads a character.
 */
void ungetch(int ch)
{
    if (ch == '\n')
	ch = ' ';
    ungetc(ch, srcfile);
    linepos--;			/* read too far, push back */
}

/*
 * error prints a message in case of an error.
 */
void error(char *str)
{
    int leng;

    fflush(stdout);
    leng = fprintf(stderr, "%s:%d:", filenam, linenum);
    leng += fprintf(stderr, "%.*s", linepos, linebuf);	/* echo line */
    fprintf(stderr, "\n%*s^", --leng, "");		/* caret corrected */
    fprintf(stderr, "\n%*s%s\n", leng, "", str);	/* message */
}

/*
 * print a runtime error to stderr and abort execution of the current program.
 */
void execerror(char *message, char *op)
{
    int leng = 0;
    char *ptr, *str;

    if ((ptr = strrchr(op, '/')) != 0)
	ptr++;
    else
	ptr = op;
    if ((str = strrchr(ptr, '.')) != 0 && str[1] == 'c')
	leng = str - ptr;
    else
	leng = strlen(ptr);
    fflush(stdout);
    fprintf(stderr, "%s:run time error: %s needed for %.*s\n", filenam, message,
	    leng, ptr);
    abortexecution_(ABORT_RETRY);
}

/*
 * redirect - read from another file descriptor.
 */
static void redirect(FILE *fp, char *name)
{
    infile[ilevel].line = linenum;		/* save last line number */
    if (ilevel + 1 == INPSTACKMAX)		/* increase include level */
	execerror("fewer include files", "include");
    infile[++ilevel].fp = srcfile = fp;		/* use new file pointer */
    infile[ilevel].line = linenum = 1;		/* start with line 1 */
    strncpy(infile[ilevel].name, name, FILENAMEMAX);
    filenam = infile[ilevel].name;		/* switch to new file */
}

/*
 * include - insert the contents of a file in the input.
 *
 * Files are read in the current directory or if that fails from the previous
 * location. If that also fails an error is generated.
 */
void include(pEnv env, char *name)
{
    FILE *fp;
    char *path = 0, *str = name;

    /*
     * usrlib.joy is tried first in the current directory, then in the home
     * directory and then in the directory where the joy binary is located.
     *
     * If all of that fails, no harm done.
     */
    if (!strcmp(name, "usrlib.joy")) {			/* check usrlib.joy */
	if ((fp = fopen(name, "r")) != 0)
	    goto normal;
	if ((path = getenv("HOME")) != 0) {		/* unix/cygwin */
	    str = GC_malloc_atomic(strlen(path) + strlen(name) + 2);
	    sprintf(str, "%s/%s", path, name);
	    if ((fp = fopen(str, "r")) != 0)
		goto normal;
	}
	if ((path = getenv("USERPROFILE")) != 0) {	/* windows */
	    str = GC_malloc_atomic(strlen(path) + strlen(name) + 2);
	    sprintf(str, "%s/%s", path, name);
	    if ((fp = fopen(str, "r")) != 0)
		goto normal;
	}
	path = env->pathname;				/* joy binary */
	if (strcmp(path, ".")) {
	    str = GC_malloc_atomic(strlen(path) + strlen(name) + 2);
	    sprintf(str, "%s/%s", path, name);
	    if ((fp = fopen(str, "r")) != 0)
		goto normal;
	}
	/*
	 * Failure to open usrlib.joy need not be reported.
	 */
	return;
normal:
	/*
	 * If there is a new pathname, replace the old one.
	 */
	if (strrchr(str, '/')) {
	    env->pathname = GC_strdup(str);
	    path = strrchr(env->pathname, '/');
	    *path = 0;
	}
	redirect(fp, name);
	return;
    }
    /*
     * A file other than usrlib.joy is tried first in the current directory.
     */
    if ((fp = fopen(name, "r")) != 0)
	goto normal;
    /*
     * If that fails, the pathname is prepended and the file is tried again.
     */
    path = env->pathname;
    if (strcmp(path, ".")) {
	str = GC_malloc_atomic(strlen(path) + strlen(name) + 2);
	sprintf(str, "%s/%s", path, name);
	if ((fp = fopen(str, "r")) != 0)
	    goto normal;
    }
    /*
     * If that also fails, no other path can be tried and an error is
     * generated.
     */
    execerror("valid file name", "include");
}

/*
 * special reads a character escape sequence.
 */
static int special(pEnv env)
{
    int ch, i, my_num;

    ch = getch(env);
    switch (ch) {
    case 'b':
	return '\b';
    case 't':
	return '\t';
    case 'n':
	return '\n';
    case 'v':
	return '\v';
    case 'f':
	return '\f';
    case 'r':
	return '\r';
    case '\"':
	return '\"';
    case '\'':
	return '\'';
    case '\\':
	return '\\';
    default:
	if (isdigit(ch)) {
	    my_num = ch - '0';
	    for (i = 0; i < 2; i++) {
		ch = getch(env);
		if (!isdigit(ch)) {
		    error("digit expected");
		    ungetch(ch);		/* not a digit, push back */
		    break;
		}
		my_num = 10 * my_num + ch - '0';
	    }
	    return my_num;
	}
	return ch;
    }
}

/*
 * getsym reads the next symbol from code or from srcfile. The return value is
 * the character after the symbol that was read.
 */
static int my_getsym(pEnv env, int ch)
{
    static char exclude[] = "\"#'().;[]{}", include[] = "-=_";
    char *ptr;
    int i, sign, type;

    vec_setsize(env->string, 0);
start:
    startpos = linepos;		/* start position of a token */
    while (ch <= ' ')
	ch = getch(env);
    switch (ch) {
    case '(':
	ch = getch(env);
	if (ch == '*') {
	    ch = getch(env);
	    do {
		while (ch != '*')
		    ch = getch(env);
		ch = getch(env);
	    } while (ch != ')');
	    ch = getch(env);
	    goto start;
	}
	env->sym = '(';
	return ch;

    case '#':
	do
	    ch = getch(env);
	while (ch != '\n');
	goto start;

    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '.':
    case ';':
	env->sym = ch;
	return getch(env);	/* read past ch */

    case '\'':
	ch = getch(env);
	if (ch == '\\')
	    ch = special(env);
	env->num = ch;
	env->sym = CHAR_;
	return getch(env);	/* read past ch */

    case '"':
	ch = getch(env);
	while (ch != '"') {
	    if (ch == '\\')
		ch = special(env);
	    vec_push(env->string, ch);
	    ch = getch(env);
	}
	vec_push(env->string, 0);
	env->str = GC_strdup(&vec_at(env->string, 0));
	env->sym = STRING_;
	return getch(env);	/* read past " */

    default:
	vec_push(env->string, ch);
	sign = ch;		/* possible sign */
	ch = getch(env);
	type = isdigit(sign) || (sign == '-' && isdigit(ch));	/* numeric */
	while (ch > ' ' && !strchr(exclude, ch)) {
	    vec_push(env->string, ch);
	    ch = getch(env);
	}
	if (ch != '.')
	    goto done;
	ch = getch(env);
	if (type) {
	    if (!isdigit(ch))	/* test float */
		goto undo;
	    type = 2;		/* floating point */
	} else if (!isalnum(ch) && !strchr(include, ch))	/* member */
	    goto undo;
	vec_push(env->string, '.');
	while (ch > ' ' && !strchr(exclude, ch)) {
	    vec_push(env->string, ch);
	    ch = getch(env);
	}
	goto done;
undo:
	ungetch(ch);		/* read too far, push back */
	ch = '.';		/* restore full stop */
done:
	vec_push(env->string, 0);
	if (type) {
	    if (type == 2) {
		env->dbl = strtod(&vec_at(env->string, 0), &ptr);
		env->sym = FLOAT_;
	    } else {
		if (sign == '-')
		    env->num = strtoll(&vec_at(env->string, 1), &ptr, 0);
		else
		    env->num = strtoll(&vec_at(env->string, 0), &ptr, 0);
		if (env->num == MAXINT_) {
		    env->dbl = strtod(&vec_at(env->string, 0), &ptr);
		    env->sym = FLOAT_;
		} else {
		    if (sign == '-')
			env->num = -env->num;
		    env->sym = INTEGER_;
		}
	    }
	    if (*ptr) {
		vec_push(env->pushback, ch);
		for (i = strlen(ptr) - 1; i >= 0; i--)
		    vec_push(env->pushback, ptr[i]);
		return getch(env);
	    }
	    return ch;
	}
	type = vec_at(env->string, 1);
	if (isupper(type) || type == '=') {
	    type = sizeof(keywords) / sizeof(keywords[0]);
	    for (sign = 0; sign < type; sign++)
		if (!strcmp(&vec_at(env->string, 0), keywords[sign].name)) {
		    env->sym = keywords[sign].sym;
		    return ch;
		}
	}
	env->str = GC_strdup(&vec_at(env->string, 0));
	env->sym = USR_;
	return ch;
    }
}

static void dumptok(pEnv env)
{
    printf("(%d,%d) ", linenum, startpos);
    switch (env->sym) {
    case CHAR_    : printf("%d", (int)env->num);
		    break;
    case STRING_  : printf("\"%s\"", env->str);
		    break;
    case FLOAT_   : printf("%g", env->dbl);
		    break;
    case INTEGER_ : printf("%" PRId64, env->num);
		    break;
    case USR_     : printf("%s", env->str);
		    break;
    case '['      : printf("LBRACK");
		    break;
    case ']'      : printf("RBRACK");
		    break;
    case '{'      : printf("LBRACE");
		    break;
    case '}'      : printf("RBRACE");
		    break;
    case '('      : printf("LPAREN");
		    break;
    case ')'      : printf("RPAREN");
		    break;
    case '.'      : printf("PERIOD");
		    break;
    case ';'      : printf("SEMICOL");
		    break;
    case LIBRA    : printf("LIBRA");
		    break;
    case EQDEF    : printf("EQDEF");
		    break;
    case HIDE     : printf("HIDE");
		    break;
    case IN__     : printf("IN");
		    break;
    case MODULE_  : printf("MODULE");
		    break;
    case PRIVATE  : printf("PRIVATE");
		    break;
    case PUBLIC   : printf("PUBLIC");
		    break;
    case CONST_   : printf("CONST");
		    break;
    }
    printf("\n");
}

/*
 * push_sym - push a symbol into the tokenlist.
 */
static void push_sym(pEnv env)
{
    Node node;

    node.op = env->sym;
    switch (node.op) {
    case CHAR_:
    case INTEGER_:
	node.u.num = env->num;
	break;
    case FLOAT_:
	node.u.dbl = env->dbl;
	break;
    case STRING_:
    case USR_:
	node.u.str = env->str;
	break;
    }
    vec_push(env->tokens, node);
}

/*
 * getsym - wrapper around my_getsym, storing tokens read, reading from the
 *	    store or just calling my_getsym itself. This allows tokens to be
 *	    read twice.
 *
 * After reading MODULE or PRIVATE, read all tokens upto END, and include them
 * in the tokenlist. All symbols preceding "==" are declared.
 */
int getsym(pEnv env, int ch)
{
    Node node;
    int module = 0, private = 0, hide = 0, modl = 0, hcnt = 0;

/*
 * If there is a tokenlist, extract tokens from there.
 */
    if (vec_size(env->tokens)) {
begin:
	node = vec_pop(env->tokens);
	env->sym = node.op;
	switch (node.op) {
	case CHAR_:
	case INTEGER_:
	    env->num = node.u.num;
	    break;
	case FLOAT_:
	    env->dbl = node.u.dbl;
	    break;
	case STRING_:
	case USR_:
	    env->str = node.u.str;
	    break;
	}
	if (env->printing)
	    dumptok(env);	/* tokens from the tokenlist */
	return ch;
    }
/*
 * There is no tokenlist, use the normal procedure to get one.
 */
    ch = my_getsym(env, ch);
/*
 * There is a token available, do some extra processing, in case the token is
 * MODULE or HIDE: MODULE .. END or HIDE .. END.
 */
    if (env->sym == MODULE_ || env->sym == HIDE) {
/*
 * Copy the global variables of modl.c into local variables.
 */
	node.op = env->sym;
	savemod(&hide, &modl, &hcnt);
	do {
	    switch (env->sym) {
	    case MODULE_  : push_sym(env);
			    ch = my_getsym(env, ch);
			    if (env->sym == USR_) {
				initmod(env, env->str);
				module++;
			    } else
				error("atom expected as name of module");
			    break;
	    case HIDE	  :
	    case PRIVATE  : initpriv(env);
			    if (!module)
				private++;
			    break;
	    case IN__	  :
	    case PUBLIC   : stoppriv();
			    break;
	    case EQDEF	  : if (node.op == USR_) {
				if (!strchr(env->str, '.'))
				    env->str = classify(env, env->str);
				enteratom(env, env->str);
			    }
			    break;
	    case '.'	  : if (module) {
				exitmod();
				module--;
			    } else if (private) {
				exitpriv();
				private--;
			    }
			    if (!module && !private)
				goto einde;
			    break;
	    }
	    node.op = env->sym;	/* previous symbol */
	    push_sym(env);
	    ch = my_getsym(env, ch);
	} while (1);
/*
 * Restore the global variables in module.c from the local copies.
 */
einde:	undomod(hide, modl, hcnt);
	push_sym(env);		/* store the last symbol that was read */
	push_sym(env);		/* extra sym for the benefit of reverse */
	vec_reverse(env->tokens);
    }
/*
 * If there is a tokenlist, extract tokens from there.
 */
    if (vec_size(env->tokens))
	goto begin;
    if (env->printing)
	dumptok(env);		/* tokens read directly */
    return ch;
}
