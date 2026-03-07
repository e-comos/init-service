# E-comOS Init Service

Microkernel init service for E-comOS. Manages core system service startup and provides service registry functionality.

## Architecture

E-comOS follows pure microkernel design with only 5 system calls:
- `SYS_ipc_send_msg` - Cross-address space messaging
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

## Project Structure

```
init-service/
├── src/                    # Source files
│   ├── init.c             # Main init service
│   ├── init_entry.c       # Kernel entry point
│   ├── boot_animation.c   # Boot animation
│   └── boot_splash.c      # Boot splash screen
├── include/               # Header files
│   ├── ecomos_types.h     # System types
│   ├── syscalls.h         # System call wrappers
│   ├── boot_animation.h   # Animation interface
│   └── boot_themes.h      # Animation themes
├── tests/                 # Test programs
│   ├── simple_test.c      # Basic animation test
│   ├── test_animation.c   # Full animation test
│   └── whistler_test.c    # Whistler theme test
├── build/                 # Build output (created by make)
├── Makefile              # Build configuration
└── README.md             # This file
```

## Build

```bash
make                # Build init.o for kernel linking
make tests         # Build test programs
make clean         # Clean build files
```

Produces `init.o` object file for linking with kernel.

## Testing

Run animation tests on host system:
```bash
cd tests
./run_tests.sh
```

## Integration

Link `init.o` with your kernel and call `init_service_entry()` to start the init service.

## Notes

This is a userspace service. Bootloader and kernel are in separate repositories.