#ifndef DRIVER_LOADER_H
#define DRIVER_LOADER_H

#include "ecomos_types.h"
#include "syscalls.h"

#define DRIVER_BASE       0x800000  // Choose a clean virtual base above EBTS
#define DRIVER_STACK_TOP  (DRIVER_BASE + 0x100000)

int driver_load(uint64_t src_addr, uint64_t size);
pcb_t* driver_prepare_pcb(service_id_t id, const char* name, uint64_t entry_point);
void driver_launch(pcb_t* pcb);

#endif
