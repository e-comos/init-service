#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "ecomos_types.h"

int  ipc_receive_msg(ipc_message_t* msg, int timeout_ms);
void thread_yield(void);
int  address_map(uint64_t vaddr, uint64_t paddr, uint32_t flags);
int  irq_wait(uint32_t irq_num);
int  thread_create(void* entry);

#endif