/*
    module  : intern.c
    version : 1.10
    date    : 03/21/24
*/
#ifndef INTERN_C
#define INTERN_C

/**
OK  2180  intern  :  "sym"  ->  sym
Pushes the item whose name is "sym".
*/
void intern_(pEnv env)
{
    int index;
    Entry ent;
    char *ptr, *str;

    ONEPARAM("intern");
    STRING("intern");
    ptr = str = nodevalue(env->stck).str;
    if (!strchr("\"#'().0123456789;[]{}", *ptr)) {
	if (*ptr == '-' && isdigit((int)ptr[1]))
	    ;
	else
	    for (ptr++; *ptr; ptr++)
		if (!isalnum((int)*ptr) && !strchr("-=_", *ptr))
		    break;
    }
    CHECKNAME(ptr, "intern");
    index = lookup(env, str);
    ent = vec_at(env->symtab, index);
    if (ent.is_user)
	UNARY(USR_NEWNODE, index);
    else {
	env->bucket.proc = ent.u.proc;
	GUNARY(ANON_FUNCT_, env->bucket);
    }
}
#endif
