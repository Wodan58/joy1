#
#   module  : makefile
#   version : 1.16
#   date    : 12/16/24
#
.POSIX:
.SUFFIXES:

# Use CC environment variable
# CC = gcc -pg
CF = -O3 -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter
LF = -lm -lgc
CFLAGS = $(CF) -DCOMP="\"$(CF)\"" -DLINK="\"$(LF)\"" -DVERS="\"BDW Release 1.0\""
HDRS = globals.h
OBJS = main.o interp.o scan.o utils.o factor.o module.o optable.o symbol.o \
       undefs.o setraw.o repl.o write.o error.o print.o

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
