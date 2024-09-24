/*
    module  : condlinrec.c
    version : 1.7
    date    : 09/17/24
*/
#ifndef CONDLINREC_C
#define CONDLINREC_C

/**
Q1  OK  2760  condlinrec  :  [ [C1] [C2] .. [D] ]  ->  ...
Each [Ci] is of the form [[B] [T]] or [[B] [R1] [R2]].
Tries each B. If that yields true and there is just a [T], executes T and exit.
If there are [R1] and [R2], executes R1, recurses, executes R2.
Subsequent case are ignored. If no B yields true, then [D] is used.
It is then of the form [[T]] or [[R1] [R2]]. For the former, executes T.
For the latter executes R1, recurses, executes R2.
*/
static void condlinrecaux(pEnv env, Node *list)
{
    int result = 0;
    Node *my_dump, *save;

    my_dump = list;
    save = env->stck;
    while (!result && my_dump && my_dump->next) {
	env->stck = save;
	exeterm(env, my_dump->u.lis->u.lis);
	result = env->stck->u.num;
	if (!result)
	    my_dump = my_dump->next;
    }
    env->stck = save;
    if (result) {
	exeterm(env, my_dump->u.lis->next->u.lis);
	if (my_dump->u.lis->next->next) {
	    condlinrecaux(env, list);
	    exeterm(env, my_dump->u.lis->next->next->u.lis);
	}
    } else {
	exeterm(env, my_dump->u.lis->u.lis);
	if (my_dump->u.lis->next) {
	    condlinrecaux(env, list);
	    exeterm(env, my_dump->u.lis->next->u.lis);
	}
    }
}

void condlinrec_(pEnv env)
{
    Node *list;

    ONEPARAM("condlinrec");
    LIST("condlinrec");
    CHECKEMPTYLIST(env->stck->u.lis, "condlinrec");
    list = env->stck->u.lis;
    POP(env->stck);
    condlinrecaux(env, list);
}
#endif
