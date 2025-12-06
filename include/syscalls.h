#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "ecomos_types.h"

// System call wrappers - implemented by kernel
extern int ipc_send(thread_id_t target, struct ipc_message* msg);
extern int ipc_receive(struct ipc_message* msg);
extern void thread_yield(void);
extern int address_map(uint32_t vaddr, uint32_t paddr, uint32_t flags);
extern int irq_wait(uint32_t irq_num);

#endif