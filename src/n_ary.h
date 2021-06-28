/*
    module  : n_ary.h
    version : 1.1
    date    : 05/21/21
*/
#ifndef N_ARY_H
#define N_ARY_H

#define N_ARY(PROCEDURE, NAME, PARAMCOUNT, TOP)                                \
    PRIVATE void PROCEDURE(pEnv env)                                           \
    {                                                                          \
        Node *save, *top;                                                      \
        PARAMCOUNT(NAME);                                                      \
        ONEQUOTE(NAME);                                                        \
        save = env->stck;                                                      \
        POP(env->stck);                                                        \
        top = TOP;                                                             \
        exeterm(env, save->u.lis);                                             \
        if (env->stck == NULL)                                                 \
            execerror(env, "value to push", NAME);                             \
        env->stck = newnode(env, env->stck->op, env->stck->u, top);            \
    }
#endif
