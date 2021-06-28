/*
    module  : condlinrec.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef CONDLINREC_C
#define CONDLINREC_C

PRIVATE void condlinrecaux(pEnv env, Node *list)
{
    int result = 0;
    Node *my_dump, *save;

    my_dump = list;
    save = env->stck;
    while (result == 0 && my_dump != NULL && my_dump->next != NULL) {
        env->stck = save;
        exeterm(env, my_dump->u.lis->u.lis);
        result = env->stck->u.num;
        if (!result)
            my_dump = my_dump->next;
    }
    env->stck = save;
    if (result) {
        exeterm(env, my_dump->u.lis->next->u.lis);
        if (my_dump->u.lis->next->next != NULL) {
            condlinrecaux(env, list);
            exeterm(env, my_dump->u.lis->next->next->u.lis);
        }
    } else {
        exeterm(env, my_dump->u.lis->u.lis);
        if (my_dump->u.lis->next != NULL) {
            condlinrecaux(env, list);
            exeterm(env, my_dump->u.lis->next->u.lis);
        }
    }
}

/**
2780  condlinrec  :  [ [C1] [C2] .. [D] ]  ->  ...
Each [Ci] is of the form [[B] [T]] or [[B] [R1] [R2]].
Tries each B. If that yields true and there is just a [T], executes T and exit.
If there are [R1] and [R2], executes R1, recurses, executes R2.
Subsequent case are ignored. If no B yields true, then [D] is used.
It is then of the form [[T]] or [[R1] [R2]]. For the former, executes T.
For the latter executes R1, recurses, executes R2.
*/
PRIVATE void condlinrec_(pEnv env)
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
