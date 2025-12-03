CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS = -m elf_i386

all: init.o

init.o: init.c ecomos_types.h syscalls.h
	$(CC) $(CFLAGS) -c init.c -o init.o

clean:
	rm -f *.o init

.PHONY: all clean