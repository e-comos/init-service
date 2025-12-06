CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
LDFLAGS = -m elf_i386

SRCDIR = src
INCDIR = include
TESTDIR = tests
OBJDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(OBJDIR) init.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

init.o: $(OBJECTS)
	ld -r $(OBJECTS) -o init.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(TESTDIR)/simple_test $(TESTDIR)/test_animation $(TESTDIR)/whistler_test

$(TESTDIR)/simple_test: $(TESTDIR)/simple_test.c
	gcc $< -o $@

$(TESTDIR)/test_animation: $(TESTDIR)/test_animation.c
	gcc $< -o $@

$(TESTDIR)/whistler_test: $(TESTDIR)/whistler_test.c
	gcc $< -o $@

clean:
	rm -rf $(OBJDIR) init.o $(TESTDIR)/simple_test $(TESTDIR)/test_animation $(TESTDIR)/whistler_test

.PHONY: all clean tests