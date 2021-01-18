/* FILE: main.c */
/*
 *  module  : main.c
 *  version : 1.29
 *  date    : 01/09/21
 */

/*
I comment on the functions in main.c which are relevant to the
symbol table management. Comments are in the same order as they
occur in main.c.

function enterglobal:

Used for most definitions, except for those inside a HIDE,
PRIVATE, PUBLIC, MODULE. Probably this should be implemented
first.
For a definition "foo == ...", enters identifier id
(string "foo") into global table. For a first use of "bar" inside
"foo == ..bar.." enters "bar" into global table. In my implementation this
part of the table uses a hashing method, the actual hash value
has already been computed by the scanner.

function lookup:

Since HIDE's and modules can be nested, there is the notion
of level (up to 10, which is ample, level 0 means global).
The array display contains the starting points of the local
tables. So, to lookup identifier id (string "foo"), start
at the current highest level (display_lookup), do a linear
search at that local table. If not found, go down one level
and repeat. If found on a local level, return (global variable
location now knows where "foo" is).
If "foo" was not found locally, search the global table which
is hashed: another linear search through the list for this
particular hash value. If not found enter into global table.
ALSO NOTE: my implementation fails to enter "foo" flagged
as is_module = 0 in the normal case. It so happens that
memory is automatically initialised to 0, so it worked. But
this was bad programming. Sorry.


function enteratom:

If we are inside a HIDE, PRIVATE or a module and see "foo == ..."
enter "foo" into table (with a NULL body), and add this
entry to the current local table at the current display level.

function definition:

Remember from that a definition is either simple (of the
form "foo == ..") or compound (HIDE, MODULE etc).
In the body of the function the first 6 lines are just for
parsing the compound ones, in each case calling function
compound_def.
The next few lines deal with empty definitions, which should
be allowed so that ";" can be used as a terminator and not just
as a separator between definitions (Pascal uses a similar trick
for statement terminator/separator).
The remainder deals with the case of atom "foo". The warning for
overwriting inbuilt should be clear, also the parsing for "==".
Then a term "..." is read, which readterm always leaves on the stack.
That is term, on top of the stack, is then attached as the body
to the part of the symbol table at here.

function defsequence:

This just parses definition sequences separated by ";".

function compound_def:

This processes in accordance with the grammar
compound-definition :==
["MODULE" atomic symbol]
["PRIVATE" definition-sequence ]
                               ^ (* ERROR: MISSING "compound_def"
                                    SEE DISGUST BELOW *)
["PUBLIC" definition-sequence]
("END" | ".")
Each of the 3 parts is optional. Instead of "PRIVATE.. PUBLIC.."
the combination "HIDE..IN.." can be used, and instead of
"PUBLIC" the syntax "LIBRA.." or "DEFINE.." can be used.
So, depending on what the symbol from the scanner is:

If the symbol is "MODULE", the next symbol must be an atom,
say "mymodule", enter it as an atom into the global or
local part of the symbol table, increment two display
indices for entering and lookup, then parse a compound definition,
which will be at one level higher than the preceding.
and then: set the is_module flag to 1 and the fields
to the current value. Finally decrement these two indices.
If the symbol is "PRIVATE" or "HIDE", the next item to be
parsed has to be a definition sequence. But the details
of what to do with the display indices depend on whether
we are already inside something that is hidden. If we are,
then only the display_enter is incremented before the
definition sequence and decremented afterwards. If we are not,
then both indices are incremented and decremented at the same
place. In either case after all that the function calls
itself. [NOTE: TO MY DISGUST, THE GRAMMAR IN THE MANUAL DOES
NOT SAY THIS - VERY SORRY.]
If the symbol is "PUBLIC" or "LIBRA" or "IN", parse and
process a definition-sequence.
If the symbol is anything else, warn about empty
compound definition.

I suppose part of the difficulty of understanding this function
is due to the fact that I tried to squeeze the earlier "HIDE..IN.."
into the same mould as the quite separate "PRIVATE.." and "PUBLIC..",
and trying to use the same display mechanism for the levels of
hiding by "HIDE" or "PRIVATE". Maybe there is a more elegant way
of doing this, possibly by separating the cases and duplicating
some of the code.

Manfred von Thun, 2006
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <gc.h>
#define ALLOC
#include "globals.h"

/*
    Initialize the symbol table with builtins.
*/
PUBLIC void inisymboltable(pEnv env) /* initialise */
{
    int i, rv;
    Entry ent;
    khiter_t key;

    symtabindex = env->symtab = GC_malloc(SYMTABMAX * sizeof(Entry));
    env->hash = kh_init(Symtab);
    for (i = 0; (ent.name = opername(i)) != 0; i++) {
        ent.is_module = ent.is_local = 0;
        ent.u.proc = operproc(i);
        ent.next = 0;
        key = kh_put(Symtab, env->hash, ent.name, &rv);
        kh_value(env->hash, key) = symtabindex;
        *symtabindex++ = ent;
    }
    firstlibra = symtabindex;
}

/*
    Global identifiers are stored at location.
*/
PRIVATE void enterglobal(pEnv env)
{
    int rv;
    Entry ent;
    khiter_t key;

    if (symtabindex - env->symtab >= SYMTABMAX)
        execerror("index", "symbols");
    ent.name = GC_strdup(ident);
    ent.is_module = ent.is_local = 0;
    ent.u.body = 0; /* may be assigned in definition */
    ent.next = 0;
    key = kh_put(Symtab, env->hash, ent.name, &rv);
    kh_value(env->hash, key) = location = symtabindex;
    *symtabindex++ = ent;
}

/*
    Lookup first searches ident in the local symbol tables; if not found in the
    global symbol table; if still not found enters ident in the global table.
*/
PUBLIC void lookup(pEnv env, int priv)
{
    int i;
    Entry ent;
    khiter_t key;

    location = 0;
    for (i = display_lookup; i > 0; i--) {
        location = display[i];
        while (location != NULL && strcmp(ident, location->name) != 0)
            location = location->next;
        if (location != NULL) /* found in local table */
            return;
    }

    if ((key = kh_get(Symtab, env->hash, ident)) != kh_end(env->hash))
        location = kh_value(env->hash, key);

    if (!priv && (!location || location->is_local))
        enterglobal(env); /* not found, enter in global */
}

/*
    Enteratom enters a symbol the symbol table, maybe a local symbol.
*/
PRIVATE void enteratom(pEnv env, int priv)
{
    int rv;
    Entry ent;
    khiter_t key;

    if (priv && display_enter > 0) {
        if (symtabindex - env->symtab >= SYMTABMAX)
            execerror("index", "symbols");
        ent.name = GC_strdup(ident);
        ent.is_local = 1;
        ent.is_module = 0;
        ent.u.body = 0; /* may be assigned later */
        ent.next = display[display_enter];
        display[display_enter] = symtabindex;
        key = kh_put(Symtab, env->hash, ent.name, &rv);
        kh_value(env->hash, key) = location = symtabindex;
        *symtabindex++ = ent;
    } else
        lookup(env, 0);
}

/*
    The rest of these procedures is not affected by the change of the symbol
    table implementation.
*/
PRIVATE void defsequence(pEnv env, int priv); /* forward */
PRIVATE void compound_def(pEnv env, int priv); /* forward */

PRIVATE void definition(pEnv env, int priv)
{
    Entry* here = NULL;

    if (symb == LIBRA || symb == JPRIVATE || symb == HIDE || symb == MODULE) {
        compound_def(env, priv);
        if (symb == END || symb == PERIOD)
            getsym(env);
        else
            error(" END or period '.' expected in compound definition");
        return;
    }

    if (symb != ATOM)
        /*   NOW ALLOW EMPTY DEFINITION:
              { error("atom expected at start of definition");
                abortexecution_(); }
        */
        return;

    /* symb == ATOM : */
    if (!priv) {
        enteratom(env, priv);
        if (location < firstlibra) {
            printf("warning: overwriting inbuilt '%s'\n", location->name);
            enterglobal(env);
        }
        here = location;
    }
    getsym(env);
    if (symb == EQDEF)
        getsym(env);
    else
        error(" == expected in definition");
    readterm(env, priv);
    if (!priv) {
        if (here != NULL)
            here->u.body = env->stck->u.lis;
        env->stck = env->stck->next;
    }
}

PRIVATE void defsequence(pEnv env, int priv)
{
    if (priv)
        enteratom(env, priv);
    definition(env, priv);
    while (symb == SEMICOL) {
        getsym(env);
        if (priv && symb <= ATOM)
            enteratom(env, priv);
        definition(env, priv);
    }
}

PRIVATE void enterdisplay(void)
{
    if (++display_enter >= DISPLAYMAX)
        execerror("index", "display");
}

PRIVATE void compound_def(pEnv env, int priv)
{
    int linenum;
    long offset;
    Entry *here = NULL, *oldplace;

    switch (symb) {
    case MODULE:
        getsym(env);
        if (symb != ATOM) {
            error("atom expected as name of module");
            abortexecution_(env);
        }
        if (!priv) {
            enteratom(env, priv);
            here = location;
        }
        getsym(env);
        ++display_lookup;
        enterdisplay();
        if (priv)
            display[display_enter] = NULL;
        compound_def(env, priv);
        if (!priv) {
            here->is_module = 1;
            here->u.module_fields = display[display_enter];
        }
        --display_enter;
        --display_lookup;
        break;
    case JPRIVATE:
    case HIDE:
        if (!priv) {
            if ((offset = ftell(srcfile)) < 0)
		execerror("fseek", "HIDE");
            linenum = getlinenum();
            compound_def(env, 1);
            if (fseek(srcfile, offset, 0))
                execerror("fseek", "HIDE");
            resetlinebuffer(linenum);
        }
        getsym(env);
        if (display_lookup > display_enter) {
            /* already inside module or hide */
            oldplace = display[display_lookup];
            /*
                        printf("lookup = %d\n",
               LOC2INT(display[display_lookup]));
                        printf("enter = %d\n", LOC2INT(display[display_enter]));
            */
            enterdisplay();
            defsequence(env, priv);
            --display_enter;
            /*
                        printf("lookup = %d\n",
               LOC2INT(display[display_lookup]));
                        printf("enter = %d\n", LOC2INT(display[display_enter]));
            */
            compound_def(env, priv);
            display[display_lookup] = oldplace;
        } else {
            ++display_lookup;
            enterdisplay();
            if (priv)
                display[display_enter] = NULL;
            defsequence(env, priv);
            --display_enter;
            compound_def(env, priv);
            --display_lookup;
        }
        break;
    case JPUBLIC:
    case LIBRA:
    case IN:
        getsym(env);
        defsequence(env, priv);
        break;
    default:
        printf("warning: empty compound definition\n");
        break;
    }
}

jmp_buf begin;

PUBLIC void abortexecution_(pEnv env) { longjmp(begin, 0); }

PUBLIC void execerror(char* message, char* op)
{
    printf("run time error: %s needed for %s\n", message, op);
    abortexecution_(0);
}

PUBLIC void quit_(pEnv env) { exit(0); }

static int mustinclude = 1;

#ifdef STATS
static void report_clock(void)
{
    double timediff;

    timediff = (double)(clock() - startclock) / CLOCKS_PER_SEC;
    fprintf(stderr, "%.2f seconds CPU to execute\n", timediff);
}
#endif

int start_main(int argc, char** argv)
{
    FILE* fp;
    Node* prog;

    Env env; /* symbol table, stack, and buckets */

    g_argc = argc;
    g_argv = argv;
    if (argc > 1) {
        g_argc--;
        g_argv++;
        srcfile = fopen(argv[1], "r");
        if (!srcfile) {
            printf("failed to open the file '%s'.\n", argv[1]);
            exit(0);
        }
        inilinebuffer(argv[1]);
        if (!strcmp(argv[1], "joytut.inp")
            || !strcmp(argv[1], "jp-joytst.joy")) {
            printf("JOY  -  compiled at 11:59:37 on Jul  2 2001 (NOBDW)\n");
            printf("Copyright 2001 by Manfred von Thun\n");
        }
        if (!strcmp(argv[1], "laztst.joy")) {
            printf("JOY  -  compiled at 15:32:32 on Nov 12 2001 (BDW)\n");
            printf("Copyright 2001 by Manfred von Thun\n");
        }
        if (!strcmp(argv[1], "lsptst.joy") || !strcmp(argv[1], "plgtst.joy")
            || !strcmp(argv[1], "symtst.joy")) {
            printf("JOY  -  compiled at 14:54:45 on Feb  1 2002 (BDW)\n");
            printf("Copyright 2001 by Manfred von Thun\n");
        }
        if (!strcmp(argv[1], "grmtst.joy") || !strcmp(argv[1], "mtrtst.joy")) {
            printf("JOY  -  compiled at 15:19:20 on Apr  3 2002 (BDW)\n");
            printf("Copyright 2001 by Manfred von Thun\n");
        }
        if (!strcmp(argv[1], "modtst.joy")) {
            printf("JOY  -  compiled at 16:57:51 on Mar 17 2003 (BDW)\n");
            printf("Copyright 2001 by Manfred von Thun\n");
        }
    } else {
        srcfile = stdin;
        inilinebuffer(0);
#ifdef GC_BDW
        printf("JOY  -  compiled at %s on %s (BDW)\n", __TIME__, __DATE__);
#else
        printf("JOY  -  compiled at %s on %s (MINBDW)\n", __TIME__, __DATE__);
#endif
        printf("Copyright 2001 by Manfred von Thun\n");
    }
    startclock = clock();
#ifdef STATS
    atexit(report_clock);
#endif
    echoflag = INIECHOFLAG;
    tracegc = INITRACEGC;
    autoput = INIAUTOPUT;
    inisymboltable(&env);
    display[0] = NULL;
    env.stck = NULL;
    setjmp(begin);
    setbuf(stdout, 0);
    while (1) {
        if (mustinclude) {
            mustinclude = 0;
            if ((fp = fopen("usrlib.joy", "r")) != 0) {
                fclose(fp);
                doinclude("usrlib.joy");
            }
        }
        getsym(&env);
        if (symb == LIBRA || symb == HIDE || symb == MODULE)
            compound_def(&env, 0);
        else {
            readterm(&env, 0);
            if (env.stck != NULL) {
                prog = env.stck->u.lis;
                env.stck = env.stck->next;
                exeterm(&env, prog);
            }
            if (autoput == 2 && env.stck != NULL) {
                writeterm(&env, env.stck, stdout);
                printf("\n");
            } else if (autoput == 1 && env.stck != NULL) {
                writefactor(&env, env.stck, stdout);
                printf("\n");
                env.stck = env.stck->next;
            }
        }
    }
}

int main(int argc, char** argv)
{
    int (*volatile m)(int, char**) = start_main;

#ifdef GC_BDW
    GC_INIT();
#else
    GC_init(&argc);
#endif
    return (*m)(argc, argv);
}
