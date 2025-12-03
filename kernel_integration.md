# Kernel Integration Guide

## Requirements for C Kernel

Your kernel must implement these components to integrate with init service:

### 1. System Call Handler
```c
// In your kernel's interrupt handler (int 0x80)
void syscall_handler(struct registers* regs) {
    switch (regs->eax) {
        case SYS_IPC_SEND:
            regs->eax = sys_ipc_send(regs->ebx, (struct ipc_message*)regs->ecx);
            break;
        case SYS_IPC_RECEIVE:
            regs->eax = sys_ipc_receive((struct ipc_message*)regs->ebx);
            break;
        case SYS_THREAD_YIELD:
            schedule();
            break;
        case SYS_ADDRESS_MAP:
            regs->eax = sys_address_map(regs->ebx, regs->ecx, regs->edx);
            break;
        case SYS_IRQ_WAIT:
            regs->eax = sys_irq_wait(regs->ebx);
            break;
    }
}
```

### 2. Process Management
```c
// Process structure (minimal)
struct process {
    uint32_t pid;
    uint32_t* page_directory;
    struct registers context;
    struct ipc_message* msg_queue;
    int state; // RUNNING, BLOCKED, etc.
};

// Create init process (PID 0)
void create_init_process(void) {
    struct process* init = create_process(0, init_main);
    schedule_process(init);
}
```

### 3. IPC Implementation
```c
int sys_ipc_send(thread_id_t target, struct ipc_message* msg) {
    struct process* target_proc = get_process(target);
    if (!target_proc) return -1;
    
    // Copy message to target's queue
    copy_message(target_proc, msg);
    
    // Wake up target if blocked on receive
    if (target_proc->state == BLOCKED_IPC) {
        target_proc->state = READY;
    }
    
    return 0;
}

int sys_ipc_receive(struct ipc_message* msg) {
    struct process* current = get_current_process();
    
    if (has_pending_message(current)) {
        copy_message_to_user(msg, get_next_message(current));
        return 0;
    }
    
    // Block until message arrives
    current->state = BLOCKED_IPC;
    schedule();
    return 0;
}
```

## Integration Steps

1. **Link init.o with kernel**:
   ```makefile
   kernel.bin: kernel.o init.o
       ld -m elf_i386 -T kernel.ld kernel.o init.o -o kernel.bin
   ```

2. **Start init process after kernel boot**:
   ```c
   void kernel_main(void) {
       // Initialize kernel subsystems
       init_gdt();
       init_idt();
       init_paging();
       
       // Create and start init process
       create_init_process();
       
       // Start scheduler
       enable_interrupts();
       while(1) halt();
   }
   ```

3. **Ensure init service can create other processes**:
   - Implement process creation syscall or
   - Pre-create service processes and let init wake them

## Memory Layout
```
0x00100000  - Kernel code
0x00200000  - Init service code  
0x00300000  - Service processes
0x00400000  - User processes
```

## Testing
1. Boot kernel
2. Verify init process starts (PID 0)
3. Check service registration messages
4. Test IPC between services