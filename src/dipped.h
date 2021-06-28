/*
    module  : dipped.h
    version : 1.1
    date    : 05/21/21
*/
#ifndef DIPPED_H
#define DIPPED_H

#define DIPPED(PROCEDURE, NAME, PARAMCOUNT, ARGUMENT)                          \
    PRIVATE void PROCEDURE(pEnv env)                                           \
    {                                                                          \
        Node *save;                                                            \
        PARAMCOUNT(NAME);                                                      \
        save = env->stck;                                                      \
        POP(env->stck);                                                        \
        ARGUMENT(env);                                                         \
        GNULLARY(save->op, save->u);                                           \
    }
#endif
