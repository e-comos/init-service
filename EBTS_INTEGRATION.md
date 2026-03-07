# EBTS Shell Integration Guide

## 1. Add EBTS entry point declaration

In `src/init.c`, add at the top:

```c
// EBTS entry point
extern void ebts_service_entry(void);
```

## 2. Modify main() function

In `src/init.c`, modify the main function:

```c
int main(void) {
    struct ipc_message msg;
    
    // Show boot animation
    show_boot_animation();
    
    // Initialize core services
    start_core_services();
    
    // Start EBTS shell
    ebts_service_entry();
    
    // Main service loop - handle service registration and lookup
    while (1) {
        if (ipc_receive(&msg) == 0) {
            handle_service_message(&msg);
        }
        thread_yield();
    }
    
    return 0;
}
```

## 3. EBTS entry point for E-comOS

In EBTS `src/main.c`:

```c
#ifdef ECOMOS_BUILD
#include "../init-service/include/ecomos_types.h"
#include "../init-service/include/syscalls.h"

void ebts_main(void) {
    // Register as shell service with init
    struct ipc_message msg;
    msg.sender = 5; // EBTS process ID
    msg.size = 40;
    msg.data[0] = MSG_SERVICE_REGISTER;
    msg.data[1] = SERVICE_SHELL;
    strcpy((char*)&msg.data[8], "ebts_shell");
    ipc_send_msg(0, &msg); // Send to init process
    
    // Start normal EBTS operation
    main();
}
#endif
```

## 3. Build Integration

Modify init-service Makefile:

```makefile
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
LDFLAGS = -m elf_i386

SRCDIR = src
INCDIR = include
OBJDIR = build
EBTS_DIR = ../ebts

SOURCES = $(wildcard $(SRCDIR)/*.c)
EBTS_SOURCES = $(EBTS_DIR)/src/main.c $(EBTS_DIR)/src/shell/shell.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EBTS_OBJECTS = $(OBJDIR)/ebts_main.o $(OBJDIR)/ebts_shell.o

all: $(OBJDIR) init.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

init.o: $(OBJECTS) $(EBTS_OBJECTS)
	ld -r $(OBJECTS) $(EBTS_OBJECTS) -o init.o

$(OBJDIR)/ebts_%.o: $(EBTS_DIR)/src/%.c
	$(CC) $(CFLAGS) -DECOMOS_BUILD -c $< -o $@

$(OBJDIR)/ebts_%.o: $(EBTS_DIR)/src/shell/%.c
	$(CC) $(CFLAGS) -DECOMOS_BUILD -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) init.o

.PHONY: all clean
```

## 4. Kernel Integration

Include EBTS in kernel linking:

```bash
# Build init service (includes EBTS)
cd init-service
make

# Build kernel (includes init + EBTS)
cd ../kernel
ld.lld -T kernel.ld kernel_entry.o src/kernel/main.o src/ipc/ipc.o src/mm/mm.o src/sched/sched.o ../init-service/init.o -o ecomos-kernel.bin
```

## 5. Boot Sequence

1. Kernel starts
2. Calls `init_service_entry()`
3. Shows boot animation
4. Starts core services (memory, display, keyboard, filesystem)
5. Starts EBTS shell
6. User can use shell commands

## Additional Requirements

**EBTS uses standard C library, init-service uses freestanding environment.**

**Solution: Use different CFLAGS for each component:**

```makefile
# Init service: freestanding (no stdlib)
INIT_CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude

# EBTS: normal C environment
EBTS_CFLAGS = -Wall -Wextra -std=c99 -g -DECOMOS_BUILD -I../init-service/include

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(INIT_CFLAGS) -c $< -o $@

$(OBJDIR)/ebts_%.o: $(EBTS_DIR)/src/%.c
	$(CC) $(EBTS_CFLAGS) -c $< -o $@
```

**This allows EBTS to use `malloc`, `printf`, `scanf` normally while keeping init service minimal.**