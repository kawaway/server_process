DRIVER=server

INCLUDES = -I. -I../deps/include
LIBS = -L../deps/lib -luv -lhttp_parser
CC = gcc
CFLAGS =  -pipe -std=gnu99 -O -Wall -Wextra -Wpointer-arith -Wconditional-uninitialized -Wno-unused-parameter -Wno-deprecated-declarations -Werror -g

SRCS=$(shell ls *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
DEPENDS=$(patsubst %.c,%.d,$(SRCS))

.SUFFIXES:	.c .o
.PHONY:	all clean

all: $(DRIVER)

$(DRIVER): $(OBJS)
	$(CC) $(CFLAGS) -o $(DRIVER) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $(INCLUDES) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

-include $(DEPENDS)

clean:	
	rm -rf $(DRIVER) *.o *.d *~

