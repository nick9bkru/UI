SHELL = /bin/sh

CC = g++
PJ = libraps
AR = ar
ARFLAGS = rv

CDEBUG = 
CFLAGS = $(CDEBUG) -Iinclude -I`pg_config --includedir`
CFLAGS_1 = $(CDEBUG) -Iinclude -I`pg_config --includedir` -O2 -fPIC
LDFLAGS = -m elf_i386 -shared -soname 
LIBS = -L`pg_config --libdir`

INSTALL = install -c
INSTALLDATA = install -c -m 644

libdir = lib
srcdir = src
objdir = obj

SRCS = $(srcdir)/*.cpp

all:	$(libdir)/$(PJ).a $(libdir)/$(PJ).so.1.0

$(libdir)/$(PJ).a: $(SRCS)
	$(CC) -c $(CFLAGS) $?
	$(AR) $(ARFLAGS) $@ *.o
	rm -f *.o

$(libdir)/$(PJ).so.1.0:$(SRCS)
	$(CC) -c $(CFLAGS_1) $?
	ld $(LDFLAGS) $(libdir)/$(PJ).so.1 -o $(libdir)/$(PJ).so.1.0 *.o
	rm -f *.o
	ln -sf $(libdir)/$(PJ).so.1.0 $(libdir)/$(PJ).so.1
	ln -sf $(libdir)/$(PJ).so.1 $(libdir)/$(PJ).so

install: all
	$(INSTALL) $(PJ) $(bindir)/$(binprefix)$(PJ)

TAGS:   $(SRCS)
	etags $(SRCS)

indent:
	indent -kr -ts4 *.c

clean:
	rm -f *.o *~ *.tmp *.bak \#* .\#* $(libdir)/$(PJ).a $(libdir)/$(PJ).so*
