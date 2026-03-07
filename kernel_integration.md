# Kernel Integration Guide

## Requirements for C Kernel

Your kernel must implement these components to integrate with init service:

### 1. System Call Handler
```c
// In your kernel's interrupt handler (int 0x80)
void syscall_handler(struct registers* regs) {
    switch (regs->eax) {
        case SYS_ipc_send_msg:
            regs->eax = sys_ipc_send_msg(regs->ebx, (struct ipc_message*)regs->ecx);
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
int sys_ipc_send_msg(thread_id_t target, struct ipc_message* msg) {
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

1. **Create init as kernel thread, not separate process**:
   ```c
   void kernel_main(void) {
       // Initialize kernel subsystems
       init_gdt();
       init_idt();
       init_paging();
       init_scheduler();
       
       // Call init service directly (runs in kernel space)
       init_service_entry();
       
       // Should not reach here - init takes over
       while(1) halt();
   }
   ```

2. **Init service creates user processes**:
   - Init runs in kernel space initially
   - Creates actual service processes in user space
   - Manages process lifecycle

3. **System calls for user processes only**:
   - Init service uses kernel functions directly
   - User services use system calls (int 0x80)

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