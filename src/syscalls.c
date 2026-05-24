#include "syscalls.h"

// 64-bit System V AMD64 ABI syscall convention:
// syscall number: rax
// args: rdi, rsi, rdx, r10, r8, r9
// return: rax

int ipc_send_msg_msg(uint32_t type, uint32_t flags, uint32_t receiver_pid,
                     uint32_t data_len, const void* data) {
    int64_t result;
    __asm__ volatile (
        "syscall"
        : "=a" (result)
        : "a" ((uint64_t)SYS_IPC_SEND_MSG),
          "D" ((uint64_t)type),
          "S" ((uint64_t)flags),
          "d" ((uint64_t)receiver_pid),
          "r" ((uint64_t)data_len),
          "r" ((uint64_t)(unsigned long long)data)
        : "rcx", "r11", "memory"
    );
    return (int)result;
}

int ipc_receive_msg(ipc_message_t* msg, int timeout_ms) {
    int64_t result;
    __asm__ volatile (
        "syscall"
        : "=a" (result)
        : "a" ((uint64_t)SYS_IPC_RECEIVE),
          "D" ((uint64_t)(unsigned long long)msg),
          "S" ((uint64_t)timeout_ms)
        : "rcx", "r11", "memory"
    );
    return (int)result;
}

void thread_yield(void) {
    __asm__ volatile (
        "syscall"
        :
        : "a" ((uint64_t)SYS_THREAD_YIELD)
        : "rcx", "r11"
    );
}

int address_map(uint64_t vaddr, uint64_t paddr, uint32_t flags) {
    int64_t result;
    __asm__ volatile (
        "syscall"
        : "=a" (result)
        : "a" ((uint64_t)SYS_ADDRESS_MAP),
          "D" (vaddr),
          "S" (paddr),
          "d" ((uint64_t)flags)
        : "rcx", "r11", "memory"
    );
    return (int)result;
}

int irq_wait(uint32_t irq_num) {
    int64_t result;
    __asm__ volatile (
        "syscall"
        : "=a" (result)
        : "a" ((uint64_t)SYS_IRQ_WAIT),
          "D" ((uint64_t)irq_num)
        : "rcx", "r11"
    );
    return (int)result;
}
