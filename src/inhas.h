/*
    module  : inhas.h
    version : 1.1
    date    : 05/21/21
*/
#ifndef INHAS_H
#define INHAS_H

#define INHAS(PROCEDURE, NAME, AGGR, ELEM)                                     \
    PRIVATE void PROCEDURE(pEnv env)                                           \
    {                                                                          \
        int found = 0, error;                                                  \
        TWOPARAMS(NAME);                                                       \
        switch (AGGR->op) {                                                    \
        case SET_:                                                             \
            found = ((AGGR->u.set) & ((long_t)1 << ELEM->u.num)) > 0;          \
            break;                                                             \
        case STRING_: {                                                        \
            char *s;                                                           \
            for (s = AGGR->u.str; s && *s != '\0' && *s != ELEM->u.num; s++)   \
                ;                                                              \
            found = s && *s != '\0';                                           \
            break;                                                             \
        }                                                                      \
        case LIST_: {                                                          \
            Node *n = AGGR->u.lis;                                             \
            while (n != NULL && (Compare(env, n, ELEM, &error) || error))      \
                n = n->next;                                                   \
            found = n != NULL;                                                 \
            break;                                                             \
        }                                                                      \
        default:                                                               \
            BADAGGREGATE(NAME);                                                \
        }                                                                      \
        BINARY(BOOLEAN_NEWNODE, found);                                        \
    }
#endif
