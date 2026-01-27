#
#   module  : makefile
#   version : 1.23
#   date    : 01/24/26
#
.POSIX:
.SUFFIXES:

CC = gcc
CF = -DRUNBYTES -I/usr/include/gc -O3 -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter
LF = -lm -lgc
CFLAGS = $(CF) -DCOMP="\"$(CF)\"" -DLINK="\"$(LF)\"" -DVERS="\"BDW Release 1.0\""
HDRS = globals.h
OBJS = main.o interp.o scan.o utils.o factor.o module.o optable.o symbol.o \
       undefs.o setraw.o repl.o write.o error.o print.o readbyte.o

joy:	prep $(OBJS)
	$(CC) -o$@ $(OBJS) $(LF)

$(OBJS): $(HDRS)

prep:
	sh prims.sh .
	sh table.sh .

clean:
	rm -f $(OBJS)

.SUFFIXES: .c .o

.c.o:
	$(CC) -o$@ $(CFLAGS) -c $<
