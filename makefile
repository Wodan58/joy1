#
#   module  : makefile
#   version : 1.6
#   date    : 09/13/23
#
.SUFFIXES:
.SUFFIXES: .c .o

CC     = gcc
CFLAGS = -DCOPYRIGHT -O3 -Wall -Wextra -Wno-unused-parameter -Werror \
	 -DJVERSION="\"BDW Release 1.0\""
HDRS   = globals.h
OBJS   = interp.o scan.o utils.o main.o factor.o module.o

joy:	prep $(OBJS)
	$(CC) -o$@ $(OBJS) -lm -lgc

$(OBJS): $(HDRS)

prep:
	sh prims.sh .
	sh table.sh .

clean:
	rm -f $(OBJS) builtin.* table.c

.c.o:
	SOURCE_DATE_EPOCH=1047920271 $(CC) -o$@ $(CFLAGS) -c $<
