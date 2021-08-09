/*
    module  : undefs.c
    version : 1.2
    date    : 07/20/21
*/
#ifndef UNDEFS_C
#define UNDEFS_C

/**
1110  undefs  :  ->  [..]
Push a list of all undefined symbols in the current symbol table.
*/
PRIVATE void undefs_(pEnv env)
{
    pEntry i;
    Entry ent;
    Index *my_dump;

    NULLARY(LIST_NEWNODE, 0);
    my_dump = &nodevalue(env->stck).lis;
    for (i = 0; i < symtabindex; i++) {
        ent = vec_at(env->symtab, i);
        if (ent.name[0] && ent.name[0] != '_' && !ent.u.body) {
            *my_dump = STRING_NEWNODE(ent.name, 0);
            my_dump = &nextnode1(*my_dump);
        }
    }
}
#endif
