#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "ecomos_types.h"

// System call wrappers
static inline int ipc_send(thread_id_t target, struct ipc_message* msg) {
    int result;
    asm volatile (
        "int $0x80"
        : "=a" (result)
        : "a" (SYS_IPC_SEND), "b" (target), "c" (msg)
        : "memory"
    );
    return result;
}

static inline int ipc_receive(struct ipc_message* msg) {
    int result;
    asm volatile (
        "int $0x80"
        : "=a" (result)
        : "a" (SYS_IPC_RECEIVE), "b" (msg)
        : "memory"
    );
    return result;
}

static inline void thread_yield(void) {
    asm volatile (
        "int $0x80"
        :
        : "a" (SYS_THREAD_YIELD)
    );
}

static inline int address_map(uint32_t vaddr, uint32_t paddr, uint32_t flags) {
    int result;
    asm volatile (
        "int $0x80"
        : "=a" (result)
        : "a" (SYS_ADDRESS_MAP), "b" (vaddr), "c" (paddr), "d" (flags)
    );
    return result;
}

static inline int irq_wait(uint32_t irq_num) {
    int result;
    asm volatile (
        "int $0x80"
        : "=a" (result)
        : "a" (SYS_IRQ_WAIT), "b" (irq_num)
    );
    return result;
}

#endif