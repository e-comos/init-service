#include "../include/ebts_loader.h"
#include "../include/syscalls.h"

// Kernel passes EBTS binary info via this struct at a known address
typedef struct {
    uint64_t src_addr;  // Physical/virtual address of EBTS binary
    uint64_t size;      // Size in bytes
    uint64_t entry;     // Entry point offset from EBTS_BASE
} ebts_info_t;

// Single EBTS PCB
static pcb_t ebts_pcb;

static void* memcpy_local(void* dst, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) *d++ = *s++;
    return dst;
}

int ebts_load(uint64_t src_addr, uint64_t size) {
    if (!src_addr || !size || size > 0x100000) return -1;

    // Map EBTS region: 0x500000 (already identity-mapped by kernel in most setups)
    // If kernel requires explicit mapping, request it
    address_map(EBTS_BASE, EBTS_BASE, 0x3); // RW present

    // Copy binary to EBTS_BASE
    memcpy_local((void*)EBTS_BASE, (const void*)src_addr, size);
    return 0;
}

pcb_t* ebts_prepare_pcb(void) {
    ebts_pcb.pid       = SERVICE_SHELL;
    ebts_pcb.state     = PROC_READY;
    ebts_pcb.entry     = EBTS_BASE;
    ebts_pcb.stack_top = EBTS_STACK_TOP;

    // Zero context; rip and rsp set at launch
    uint8_t* p = (uint8_t*)&ebts_pcb.ctx;
    for (uint64_t i = 0; i < sizeof(cpu_context_t); i++) p[i] = 0;

    ebts_pcb.ctx.rip    = EBTS_BASE;
    ebts_pcb.ctx.rsp    = EBTS_STACK_TOP;
    ebts_pcb.ctx.rflags = 0x202; // IF set

    ebts_pcb.name[0] = 'e'; ebts_pcb.name[1] = 'b';
    ebts_pcb.name[2] = 't'; ebts_pcb.name[3] = 's';
    ebts_pcb.name[4] = '\0';

    return &ebts_pcb;
}

// Jump to EBTS entry with its own stack - does not return
void ebts_launch(pcb_t* pcb) {
    pcb->state = PROC_RUNNING;
    __asm__ volatile (
        "mov %0, %%rsp\n\t"
        "jmp *%1\n\t"
        :
        : "r" (pcb->ctx.rsp), "r" (pcb->ctx.rip)
        : "memory"
    );
    // unreachable
    while (1) { __asm__ volatile ("hlt"); }
}
