# edit DRIVER PREFIX 
DRIVER=log_driver
PREFIX = ../../src/
LINKS = log.c

CC = gcc
CFLAGS =  -pipe -std=gnu99 -O -Wall -Wextra -Wpointer-arith -Wconditional-uninitialized -Wno-unused-parameter -Wno-deprecated-declarations -Werror -g
INCLUDES = -I$(PREFIX)

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
DEPENDS=$(patsubst %.c,%.d,$(SRCS))
.SUFFIXES:	.c .o

.PHONY:	all clean
all: link $(DRIVER)

$(DRIVER): $(OBJS)
	$(CC) $(CFLAGS) -o $(DRIVER) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $(INCLUDES) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

-include $(DEPENDS)

clean:	
	rm -rf $(DRIVER) $(LINKS) *.o *.d *~

link:
	@for lnk in $(LINKS); do ln -fs $(PREFIX)$$lnk $$lnk ; done
