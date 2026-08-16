#include "../include/ebts_loader.h"
#include <syscall.h>

// Kernel passes EBTS binary info via this struct at a known address
typedef struct {
    uint64_t src_addr;  
    uint64_t size;      
    uint64_t entry;     
} ebts_info_t;

static pcb_t ebts_pcb;

static void* memcpy_local(void* dst, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) *d++ = *s++;
    return dst;
}

int ebts_load(uint64_t src_addr, uint64_t size) {
    if (!src_addr || !size || size > 0x100000) return -1;

    // 1. Map the entire program memory (code + BSS sections)
    //    The total size 0x40000 (256KB) is enough to cover the BSS end at 0x508bdc.
    //    This ensures that the .bss region (0x504ba0 ~ 0x508bdc) is mapped.
    uint64_t total_size = 0x40000;
    uint64_t pages = (total_size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t p = 0; p < pages; p++) {
        uint64_t va = EBTS_BASE + p * PAGE_SIZE;
        syscall(SYS_ADDRESS_MAP, va, va, 0x3);   // User Read/Write
    }

    // 2. Copy only the file content (.text + .rodata + .data) to the target.
    //    The .bss section is not present in the file, so it is not copied.
    memcpy_local((void*)EBTS_BASE, (const void*)src_addr, size);

    // 3. Map the stack page (unchanged from original logic)
    uint64_t stack_vaddr = EBTS_STACK_TOP - EBTS_STACK_SIZE;
    syscall(SYS_ADDRESS_MAP, stack_vaddr, stack_vaddr, 0x3); // User Read/Write

    return 0;
}

pcb_t* ebts_prepare_pcb(void) {
    ebts_pcb.pid       = SERVICE_SHELL;
    ebts_pcb.state     = PROC_READY;
    ebts_pcb.entry     = EBTS_BASE;
    ebts_pcb.stack_top = EBTS_STACK_TOP;

    uint8_t* p = (uint8_t*)&ebts_pcb.ctx;
    for (uint64_t i = 0; i < sizeof(cpu_context_t); i++) p[i] = 0;

    ebts_pcb.ctx.rip    = EBTS_BASE;
    ebts_pcb.ctx.rsp    = EBTS_STACK_TOP; // Top of the stack
    ebts_pcb.ctx.rflags = 0x202;         // Interrupts enabled

    ebts_pcb.name[0] = 'e'; ebts_pcb.name[1] = 'b';
    ebts_pcb.name[2] = 't'; ebts_pcb.name[3] = 's';
    ebts_pcb.name[4] = '\0';

    return &ebts_pcb;
}

void ebts_launch(pcb_t* pcb) {
    pcb->state = PROC_RUNNING;
    
    int64_t result;
    // Pass Entry Point (RIP) in RDI and Stack Pointer (RSP) in RSI
    __asm__ volatile (
        "syscall\n\t"
        : "=a" (result)
        : "a" ((uint64_t)SYS_THREAD_CREATE),
          "D" (pcb->ctx.rip),  // Arg 1: RIP -> RDI
          "S" (pcb->ctx.rsp),  // Arg 2: RSP -> RSI (Fixed: was 0 before)
          "d" (0)              // Arg 3: RDX
        : "rcx", "r11", "memory"
    );
}
