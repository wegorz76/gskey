CC = gcc
CFLAGS = -O2
prefix = /usr/local

LIB = skey
SRCS = skeyaccess.c put.c skey_crypt.c skey_getpass.c skeylogin.c skeysubr.c
MAN1 = skey.1
MAN5 = skey.access.5

OBJS = $(SRCS:.c=.o)
REQUIRED_CFLAGS = -DPERMIT_CONSOLE -I. -I../libmd

all: lib$(LIB).a

install: all
	install -c -m 644 $(MAN1) $(prefix)/man/man1
	install -c -m 644 $(MAN5) $(prefix)/man/man5

clean:
	rm -f $(OBJS) lib$(LIB).a

lib$(LIB).a: $(OBJS)
	ar crs lib$(LIB).a $(OBJS)

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(REQUIRED_CFLAGS) -c $<
