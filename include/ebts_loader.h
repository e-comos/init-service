#ifndef EBTS_LOADER_H
#define EBTS_LOADER_H

#include "ecomos_types.h"

#define PAGE_SIZE 4096

// Load EBTS binary from src_addr into EBTS_BASE (0x500000)
// Returns 0 on success, -1 on failure
int ebts_load(uint64_t src_addr, uint64_t size);

// Prepare PCB for EBTS process and mark it READY
// Returns pointer to PCB, or 0 on failure
pcb_t* ebts_prepare_pcb(void);

// Transfer control to EBTS (does not return on success)
void ebts_launch(pcb_t* pcb);

#endif
