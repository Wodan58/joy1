/* FILE: scan.c */
/*
 *  module  : scan.c
 *  version : 1.6
 *  date    : 01/05/19
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#ifdef GC_BDW
#    include <gc.h>
#    define malloc GC_malloc_atomic
#endif

#define EOLN '\n'

#ifdef USE_ONLY_STDIN
// FIXME: resume earlier line numbering
// <a href="http://home.kpn.nl/r.wiersma26/joy/j09imp.html#linecounter>link</a>
static struct {
    FILE *fp;
    char *name;
} infile[INPSTACKMAX];
#else
static FILE *infile[INPSTACKMAX];
#endif
static int ilevel;
static int linenumber = 0;
static char linbuf[INPLINEMAX];
static int linelength, currentcolumn = 0;
#ifndef REMOVE_UNUSED_ERRORCOUNT
static int errorcount = 0;
#endif
static int ch = ' ';
#ifdef READ_NUMBER_AND_STOP
static int unget1 = 0, unget2 = 0;
#endif
#if defined(GET_FROM_STDIN) || defined(FGET_FROM_FILE)
static int get_from_stdin = 0;
#endif

#ifdef USE_ONLY_STDIN
PUBLIC void inilinebuffer(char *str)
#else
PUBLIC void inilinebuffer(void)
#endif
{
#ifdef USE_ONLY_STDIN
    infile[0].fp = srcfile;
    infile[0].name = str;
#else
    ilevel = 0; infile[ilevel] = srcfile;
#endif
}

PUBLIC void putline(void)
{
    if (echoflag > 2) printf("%4d",linenumber);
    if (echoflag > 1) printf("\t");
    printf("%s\n",linbuf);
}

PRIVATE void getch()
{
#ifdef DONT_READ_PAST_EOF
    int c;
#else
    char c;
#endif
#ifdef READ_NUMBER_AND_STOP
    if (unget1) {
	ch = unget1;
	unget1 = unget2;
	unget2 = 0;
	return;
    }
#endif
    if (currentcolumn == linelength)
      { Again:
	currentcolumn = 0; linelength = 0;
	linenumber++;
#ifdef USE_ONLY_STDIN
	while ((c = getc(srcfile)) != EOLN)
#else
	while ((c = getc(infile[ilevel])) != EOLN)
#endif
#ifdef RESPECT_INPLINEMAX
	  { if (linelength < INPLINEMAX - 2)
		linbuf[linelength++] = c;
#else
	  { linbuf[linelength++] = c;
#endif
#ifdef DONT_READ_PAST_EOF
	    if (c == EOF) linelength--;
#endif
#ifdef USE_ONLY_STDIN
	    if (feof(srcfile))
#else
	    if (feof(infile[ilevel]))
#endif
	      { ilevel--;
#ifdef USE_ONLY_STDIN
		if (ilevel >= 0)
		    srcfile = infile[ilevel].fp;
#endif
D(		printf("reset to level %d\n",ilevel); )
		if (ilevel < 0) quit_(); } }
#ifdef DONT_READ_PAST_EOF
	if (linelength)
#endif
	linbuf[linelength++] = ' ';  /* to help getsym for numbers */
	linbuf[linelength++] = '\0';
	if (echoflag) putline();
#ifdef USE_SHELL_ESCAPE
	if (linbuf[0] == SHELLESCAPE)
	    { system(&linbuf[1 ]); goto Again; } }
#else
	}
#endif
    ch = linbuf[currentcolumn++];
}

PUBLIC int endofbuffer(void)
{
    return (currentcolumn == linelength);
}

PUBLIC void error(char *message)
{
    int i;
    putline();
    if (echoflag > 1) putchar('\t');
    for (i = 0; i < currentcolumn-2; i++)
	if (linbuf[i] <= ' ') putchar(linbuf[i]); else putchar(' ');
    printf("^\n\t%s\n",message);
#ifndef REMOVE_UNUSED_ERRORCOUNT
    errorcount++;
#endif
}

PUBLIC int doinclude(char *filnam)
{
    if (ilevel+1 == INPSTACKMAX)
	execerror("fewer include files","include");
#ifdef USE_ONLY_STDIN
    infile[ilevel].fp = srcfile;
    if ((srcfile = fopen(filnam, "r")) != 0) {
	infile[++ilevel].fp = srcfile;
	infile[ilevel].name = filnam;
	return 1;
    }
#else
    if ((infile[ilevel+1] = fopen(filnam,"r")) != NULL)
	{ ilevel++; return(1); }
#endif
    execerror("valid file name","include");
    return 0;
}

#if defined(GET_FROM_STDIN) || defined(FGET_FROM_FILE)
PUBLIC void redirect(FILE *fp)
{
#ifdef USE_ONLY_STDIN
    if (infile[ilevel].fp != fp && !get_from_stdin) {
	get_from_stdin = fp == stdin;
	if (++ilevel == INPSTACKMAX)
	    execerror("fewer include files", "redirect");
	infile[ilevel].fp = fp;
	infile[ilevel].name = 0;
    }
#else
    if (infile[ilevel] != fp && !get_from_stdin) {
	get_from_stdin = fp == stdin;
	if (++ilevel == INPSTACKMAX)
	    execerror("fewer include files", "redirect");
	infile[ilevel] = fp;
    }
#endif
}
#endif

PRIVATE int specialchar()
{
    getch();
    switch (ch)
      { case 'n' : return '\n';
	case 't' : return '\t';
	case 'b' : return '\b';
	case 'r' : return '\r';
	case 'f' : return '\f';
	case '\'': return '\'';
	case '\"': return '\"';
#ifdef REST_OF_UNIX_ESCAPES
	case 'v' : return '\v';
	case '\\': return '\\';
#endif
	default :
	    if (ch >= '0' && ch <= '9')
	      { int i, num;
		num = ch - '0';
		for (i = 0; i < 2; i++)
		  { getch();
		    if (ch < '0' || ch > '9')
		      { currentcolumn++; /* to get pointer OK */
			error("digit expected");
			currentcolumn--; }
		    num = 10 * num + ch - '0'; }
		return num; }
	    else return ch; }
}

#ifdef HASHVALUE_FUNCTION
PUBLIC void HashValue(char *s)
{
    for (hashvalue = 0; *s != '\0';) hashvalue += *s++;
    hashvalue %= HASHSIZE;
}
#endif

PUBLIC void getsym(void)
{
Start:
    while (ch <= ' ') getch();
    switch (ch)
      { case '(':
	    getch();
	    if (ch == '*')
		{ getch();
		  do {while (ch != '*') getch(); getch();}
		    while (ch != ')');
		  getch(); goto Start; }
	    else {symb = LPAREN; return;}
	case '#':
	    currentcolumn = linelength; getch(); goto Start;
	case ')':
	    symb = RPAREN; getch(); return;
	case '[':
	    symb = LBRACK; getch(); return;
	case ']':
	    symb = RBRACK; getch(); return;
	case '{':
	    symb = LBRACE; getch(); return;
	case '}':
	    symb = RBRACE; getch(); return;
	case '.':
	    symb = PERIOD; getch(); return;
	case ';':
	    symb = SEMICOL; getch(); return;
	case '\'':
	    getch();
	    if (ch == '\\') ch = specialchar();
	    numb = ch;
	    symb = CHAR_; getch(); return;
	case '"':
	  { char string[INPLINEMAX];
	    register int i = 0;
	    getch();
	    while (ch != '"' && !endofbuffer())
	      { if (ch == '\\') ch = specialchar();
#ifdef RESPECT_INPLINEMAX
		if (i < INPLINEMAX - 1)
#endif
		string[i++] = ch; getch();}
	    string[i] = '\0'; getch();
D(	    printf("getsym: string = %s\n",string); )
#ifdef NO_COMPILER_WARNINGS
	    numb = (size_t)malloc(strlen(string) + 1);
	    strcpy((char *)(size_t)numb, string);
#else
	    numb = (long) malloc(strlen(string) + 1);
	    strcpy((char *) numb, string);
#endif
	    symb = STRING_; return; }
	case '-': /* PERHAPS unary minus */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	  { char number[25];
	    char *p = number;
#ifdef CORRECT_OCTAL_NUMBER
	    int octal = ch == '0';
#endif
#ifdef READ_HEXADECIMAL_NUMBER
	    int hexal = 0;
#endif
	    if ( isdigit(ch) ||
		 ( currentcolumn < linelength &&
		   isdigit((int)linbuf[currentcolumn]) ) )
#ifdef PROTECT_BUFFER_OVERFLOW
	      { do {if (p - number < 24) *p++ = ch; getch();
#else
	      { do {*p++ = ch; getch();
#endif
#ifdef READ_HEXADECIMAL_NUMBER
		    if (hexal && !isxdigit(ch))
			break;
		    if (octal && (ch == 'x' || ch == 'X')) {
			octal = 0;
			hexal = 1;
		    }
#endif
#ifdef CORRECT_OCTAL_NUMBER
		    if (octal && ch == '.')
			octal = 0;
		    if (octal && (strchr("89", ch) || !isdigit(ch)))
			break;
#endif
#ifdef READ_HEXADECIMAL_NUMBER
		} while (strchr("0123456789+-Ee.XxAaBbCcdDdFf", ch));
#else
		} while (strchr("0123456789+-Ee.", ch));
#endif
		*p = 0;
#ifdef READ_NUMBER_AND_STOP
		if (p[-1] == '.') {
		    p[-1] = 0;
		    unget1 = '.';
		    unget2 = ch;
		}
#endif
#ifdef READ_HEXADECIMAL_NUMBER
		if (strchr(number, '.'))
#else
		if (strpbrk(number, ".eE"))
#endif
		  { dblf = strtod(number, NULL); symb = FLOAT_; return;}
		else
#ifdef BIT_32
		{ numb = strtol(number, NULL, 0); symb = INTEGER_; return; } } }
#else
		{ numb = strtoll(number, NULL,0); symb = INTEGER_; return; } } }
#endif
	    /* ELSE '-' is not unary minus, fall through */
	default:
	  { int i = 0;
	    hashvalue = 0; /* ensure same algorithm in inisymtab */
	    do { if (i < ALEN-1) {ident[i++] = ch; hashvalue += ch;}
		 getch(); }
	      while (isalpha(ch) || isdigit(ch) ||
		       ch == '=' || ch == '_' || ch == '-');
	    ident[i] = '\0'; hashvalue %= HASHSIZE;
#ifdef HASHVALUE_FUNCTION
	    HashValue(ident);
#endif
	    if (isupper((int)ident[1]))
	      { if (strcmp(ident,"LIBRA") == 0 || strcmp(ident,"DEFINE") == 0)
		  { symb = LIBRA; return; }
		if (strcmp(ident,"HIDE") == 0)
		  { symb = HIDE; return; }
		if (strcmp(ident,"IN") == 0)
		  { symb = IN; return; }
		if (strcmp(ident,"END") == 0)
		  { symb = END; return; }
		if (strcmp(ident,"MODULE") == 0)
		  { symb = MODULE; return; }
		if (strcmp(ident,"PRIVATE") == 0)
		  { symb = JPRIVATE; return; }
		if (strcmp(ident,"PUBLIC") == 0)
		  { symb = JPUBLIC; return; }
		/* possibly other uppers here */
		}
#ifndef NO_BANG_AS_PERIOD
	    if (strcmp(ident,"!") == 0) /* should this remain or be deleted ? */
	      { symb = PERIOD; return; }
#endif
	    if (strcmp(ident,"==") == 0)
	      { symb = EQDEF; return; }
	    if (strcmp(ident,"true") == 0)
	      { symb = BOOLEAN_; numb = 1; return; }
	    if (strcmp(ident,"false") == 0)
	      { symb = BOOLEAN_; numb = 0; return; }
	    if (strcmp(ident,"maxint") == 0)
	      { symb = INTEGER_; numb = MAXINT; return; }
	    symb = ATOM; return; } }
}
