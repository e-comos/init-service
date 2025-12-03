# E-comOS Init Service

Microkernel init service for E-comOS. Manages core system service startup and provides service registry functionality.

## Architecture

E-comOS follows pure microkernel design with only 5 system calls:
- `SYS_IPC_SEND` - Cross-address space messaging
- `SYS_IPC_RECEIVE` - Blocking message receive
- `SYS_THREAD_YIELD` - CPU yield to scheduler
- `SYS_ADDRESS_MAP` - Physical page mapping (privileged)
- `SYS_IRQ_WAIT` - Hardware interrupt wait

## Core Services

Init service starts these core services in order:
1. Memory Manager Service (process 1)
2. VGA Display Service (process 2) 
3. Keyboard Input Service (process 3)
4. File System Service (process 4)
5. Shell Process (process 5)

## Service Registry

Init provides service lookup via IPC messages:
- `MSG_SERVICE_REGISTER` - Register new service
- `MSG_SERVICE_LOOKUP` - Find service by ID
- `MSG_SERVICE_START` - Start service process

## Build

```bash
make
```

Produces `init.o` object file for linking with kernel.

## Files

- `init.c` - Main init service implementation
- `ecomos_types.h` - System type definitions
- `syscalls.h` - System call wrappers
- `Makefile` - Build configuration

## Notes

This is a userspace service. Bootloader and kernel are in separate repositories.