CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS = -m elf_i386

all: init.o init_entry.o

init.o: init.c ecomos_types.h syscalls.h
	$(CC) $(CFLAGS) -c init.c -o init.o

init_entry.o: init_entry.c
	$(CC) $(CFLAGS) -c init_entry.c -o init_entry.o

clean:
	rm -f *.o

.PHONY: all clean