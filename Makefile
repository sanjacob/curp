# curp makefile
SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .c .o

# commands
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# installation directories
prefix = /usr/local
exec_prefix = $(prefix)
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include

# configuration
OBJDIR := build
OBJS := $(addprefix $(OBJDIR)/,curp.o trie.o)
CHECK_FLAGS = -pthread -lcheck_pic -lrt -lm -lsubunit -ltheft

# targets
.PHONY: all
all: clean libcurp.so

libcurp.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

libcurp.so: $(OBJS)
	$(CC) $(LDFLAGS) -shared $^ -o $@

$(OBJDIR)/curp.o: src/curp.c include/curp.h include/trie.h
	$(CC) -c $(CFLAGS) -Iinclude $< -o $@

$(OBJDIR)/trie.o: src/trie.c include/trie.h
	$(CC) -c $(CFLAGS) -Iinclude $< -o $@

$(OBJDIR)/check_curp_examples.o: tests/check_curp_examples.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -Iinclude $< -o $@

$(OBJDIR)/check_curp_pbt.o: tests/check_curp_pbt.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -Iinclude $< -o $@

$(OBJDIR)/check_curp.o: tests/check_curp.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) -Iinclude $< -o $@

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: tests
tests: $(OBJDIR)/check_curp.o $(OBJDIR)/check_curp_examples.o $(OBJDIR)/check_curp_pbt.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o test $(CHECK_FLAGS)

.PHONY: install
install:
	$(INSTALL_DATA) -D libcurp.so $(DESTDIR)$(libdir)/libcurp.so
	$(INSTALL_DATA) -D include/curp.h $(DESTDIR)$(includedir)/libcurp/curp.h

.PHONY: uninstall
uninstall:
	$(RM) $(DESTDIR)$(libdir)/libcurp.so
	$(RM) $(DESTDIR)$(includedir)/libcurp/curp.h
	rmdir $(DESTDIR)$(includedir)/libcurp

.PHONY: check
check: tests
	./test

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
