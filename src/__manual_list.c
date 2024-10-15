/*
    module  : __manual_list.c
    version : 1.12
    date    : 10/11/24
*/
#ifndef __MANUAL_LIST_C
#define __MANUAL_LIST_C

/**
Q0  OK  2960  __manual_list  :  ->  L
[SYMBOLS] Pushes a list L of lists (one per operator) of three
documentation strings.
*/
void __manual_list_(pEnv env)
{
    int i, j;
    Index *my_dump, *my_dump2;

    NULLARY(LIST_NEWNODE, 0);
    my_dump = &nodevalue(env->stck).lis;
    for (i = 0, j = tablesize(); i < j; i++) {
	*my_dump = LIST_NEWNODE(0, 0);
	my_dump2 = &nodevalue(*my_dump).lis;
	*my_dump2 = STRING_NEWNODE(optable[i].name, 0);
	my_dump2 = &nextnode1(*my_dump2);
	*my_dump2 = STRING_NEWNODE(optable[i].messg1, 0);
	my_dump2 = &nextnode1(*my_dump2);
	*my_dump2 = STRING_NEWNODE(optable[i].messg2, 0);
	my_dump = &nextnode1(*my_dump);
    }
}
#endif
