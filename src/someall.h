/*
    module  : someall.h
    version : 1.1
    date    : 05/21/21
*/
#ifndef SOMEALL_H
#define SOMEALL_H

#define SOMEALL(PROCEDURE, NAME, INITIAL)                                      \
    PRIVATE void PROCEDURE(pEnv env)                                           \
    {                                                                          \
        int result = INITIAL;                                                  \
        Node *program, *my_dump, *save;                                        \
        TWOPARAMS(NAME);                                                       \
        ONEQUOTE(NAME);                                                        \
        program = env->stck->u.lis;                                            \
        POP(env->stck);                                                        \
        save = env->stck->next;                                                \
        switch (env->stck->op) {                                               \
        case SET_: {                                                           \
            int j;                                                             \
            long_t set = env->stck->u.set;                                     \
            for (j = 0; j < SETSIZE && result == INITIAL; j++) {               \
                if (set & ((long_t)1 << j)) {                                  \
                    env->stck = INTEGER_NEWNODE(j, save);                      \
                    exeterm(env, program);                                     \
                    if (env->stck->u.num != INITIAL)                           \
                        result = 1 - INITIAL;                                  \
                }                                                              \
            }                                                                  \
            break;                                                             \
        }                                                                      \
        case STRING_: {                                                        \
            char *s;                                                           \
            char *volatile ptr = GC_strdup(env->stck->u.str);                  \
            for (s = ptr; *s != '\0' && result == INITIAL; s++) {              \
                env->stck = CHAR_NEWNODE(*s, save);                            \
                exeterm(env, program);                                         \
                if (env->stck->u.num != INITIAL)                               \
                    result = 1 - INITIAL;                                      \
            }                                                                  \
            break;                                                             \
        }                                                                      \
        case LIST_: {                                                          \
            my_dump = env->stck->u.lis;                                        \
            while (my_dump != NULL && result == INITIAL) {                     \
                env->stck = newnode(env, my_dump->op, my_dump->u, save);       \
                exeterm(env, program);                                         \
                if (env->stck->u.num != INITIAL)                               \
                    result = 1 - INITIAL;                                      \
                my_dump = my_dump->next;                                       \
            }                                                                  \
            break;                                                             \
        }                                                                      \
        default:                                                               \
            BADAGGREGATE(NAME);                                                \
        }                                                                      \
        env->stck = BOOLEAN_NEWNODE(result, save);                             \
    }
#endif
