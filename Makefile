CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude -I../ECLib/include
LDFLAGS = -m elf_i386

SRCDIR = src
INCDIR = include
TESTDIR = tests
OBJDIR = build
EBTS_DIR = ../ebts

SOURCES = $(wildcard $(SRCDIR)/*.c)
EBTS_SOURCES = $(EBTS_DIR)/src/main.c $(EBTS_DIR)/src/shell/shell.c $(EBTS_DIR)/src/shell/command.c $(EBTS_DIR)/src/shell/object.c $(EBTS_DIR)/src/shell/user.c $(EBTS_DIR)/src/shell/commandbox.c $(EBTS_DIR)/src/utils/memory.c $(EBTS_DIR)/src/utils/string.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EBTS_OBJECTS = $(OBJDIR)/ebts_main.o $(OBJDIR)/ebts_shell.o $(OBJDIR)/ebts_command.o $(OBJDIR)/ebts_object.o $(OBJDIR)/ebts_user.o $(OBJDIR)/ebts_commandbox.o $(OBJDIR)/ebts_memory.o $(OBJDIR)/ebts_string.o

# Init service: freestanding (no stdlib)
INIT_CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude

# EBTS: normal C environment
EBTS_CFLAGS = -Wall -Wextra -std=c99 -g -DECOMOS_BUILD -I../init-service/include

all: $(OBJDIR) init.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

init.o: $(OBJECTS) $(EBTS_OBJECTS)
	ld -r $(OBJECTS) $(EBTS_OBJECTS) -o init.o

$(OBJDIR)/ebts_main.o: $(EBTS_DIR)/src/main.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_shell.o: $(EBTS_DIR)/src/shell/shell.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_command.o: $(EBTS_DIR)/src/shell/command.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_object.o: $(EBTS_DIR)/src/shell/object.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_user.o: $(EBTS_DIR)/src/shell/user.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_commandbox.o: $(EBTS_DIR)/src/shell/commandbox.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_memory.o: $(EBTS_DIR)/src/utils/memory.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_string.o: $(EBTS_DIR)/src/utils/string.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(INIT_CFLAGS) -c $< -o $@

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