/*
    module  : mktime.c
    version : 1.1
    date    : 05/21/21
*/
#ifndef MKTIME_C
#define MKTIME_C

#include "decode.h"

/**
1720  mktime  :  T  ->  I
Converts a list T representing local time into a time I.
T is in the format generated by localtime.
*/
PRIVATE void mktime_(pEnv env)
{
    struct tm t;

    ONEPARAM("mktime");
    LIST("mktime");
    decode_time(env, &t);
    UNARY(INTEGER_NEWNODE, (long_t)mktime(&t));
}
#endif
