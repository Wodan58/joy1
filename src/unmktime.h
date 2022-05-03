/*
    module  : unmktime.h
    version : 1.2
    date    : 07/20/21
*/
#ifndef UNMKTIME_H
#define UNMKTIME_H

#define UNMKTIME(PROCEDURE, NAME, FUNC)                                        \
    PRIVATE void PROCEDURE(pEnv env)                                           \
    {                                                                          \
        struct tm t;                                                           \
        long_t wday;                                                           \
        time_t timval;                                                         \
        Index *my_dump;                                                        \
        ONEPARAM(NAME);                                                        \
        INTEGER(NAME);                                                         \
        timval = env->stck->u.num;                                             \
        FUNC(&timval, &t);                                                     \
        if ((wday = t.tm_wday) == 0)                                           \
            wday = 7;                                                          \
        UNARY(LIST_NEWNODE, 0);                                                \
        my_dump = &nodevalue(env->stck).lis;                                   \
        *my_dump = INTEGER_NEWNODE((t.tm_year + 1900), 0);                     \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE((t.tm_mon + 1), 0);                         \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE(t.tm_mday, 0);                              \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE(t.tm_hour, 0);                              \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE(t.tm_min, 0);                               \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE(t.tm_sec, 0);                               \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = BOOLEAN_NEWNODE(t.tm_isdst, 0);                             \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE(t.tm_yday, 0);                              \
        my_dump = &nextnode1(*my_dump);                                        \
        *my_dump = INTEGER_NEWNODE(wday, 0);                                   \
    }
#endif