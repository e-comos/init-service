# Kernel and Service Integration Guide

## Kernel Integration

### 1. Build and Link
```bash
# In init-service directory:
make                    # Generate init.o

# In kernel directory:
# Build kernel objects first:
make                    # Build all kernel objects

# Link with init service:
# macOS:
ld.lld -T kernel.ld kernel_entry.o src/kernel/main.o src/ipc/ipc.o src/mm/mm.o src/sched/sched.o ~/init-service/init.o -o ecomos-kernel.bin

# Linux:
ld -T kernel.ld kernel_entry.o src/kernel/main.o src/ipc/ipc.o src/mm/mm.o src/sched/sched.o ~/init-service/init.o -o ecomos-kernel.bin
```

### 2. Kernel Call
```c
// Call after kernel initialization
extern void init_service_entry(void);

void kernel_main(void) {
    // Kernel initialization...
    
    // Start init service
    init_service_entry();
}
```

## Service Integration

### 1. Service Registration
```c
struct ipc_message msg;
msg.sender = my_process_id;
msg.size = 40;
msg.data[0] = MSG_SERVICE_REGISTER;
msg.data[1] = MY_SERVICE_ID;
strcpy((char*)&msg.data[8], "my_service");
ipc_send_msg(0, &msg);  // Send to init process
```

### 2. Service Lookup
```c
struct ipc_message msg, response;
msg.sender = my_process_id;
msg.size = 8;
msg.data[0] = MSG_SERVICE_LOOKUP;
msg.data[1] = SERVICE_FILE_SYSTEM;
ipc_send_msg(0, &msg);
ipc_receive(&response);
process_id_t fs_pid = *((process_id_t*)&response.data[4]);
```

### 3. Adding New Services
Add to `include/ecomos_types.h`:
```c
#define SERVICE_MY_NEW_SERVICE  6
```

Add to `start_core_services()` in `src/init.c`:
```c
msg.data[1] = SERVICE_MY_NEW_SERVICE;
ipc_send_msg(SERVICE_MY_NEW_SERVICE, &msg);
register_service(SERVICE_MY_NEW_SERVICE, SERVICE_MY_NEW_SERVICE, "my_new_service");
```