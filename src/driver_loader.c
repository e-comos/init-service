#include "../include/driver_loader.h"

static pcb_t driver_pcb;

static void* memcpy_local(void* dst, const void* src, uint64_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) *d++ = *s++;
    return dst;
}

int driver_load(uint64_t src_addr, uint64_t size) {
    if (!src_addr || !size || size > 0x100000) return -1;

    address_map(DRIVER_BASE, DRIVER_BASE, 0x3); 
    memcpy_local((void*)DRIVER_BASE, (const void*)src_addr, size);
    return 0;
}

pcb_t* driver_prepare_pcb(service_id_t id, const char* name, uint64_t entry_point) {
    driver_pcb.pid         = id;
    driver_pcb.state       = PROC_READY;
    driver_pcb.entry       = entry_point;
    driver_pcb.stack_top   = DRIVER_STACK_TOP;

    uint8_t* p = (uint8_t*)&driver_pcb.ctx;
    for (uint64_t i = 0; i < sizeof(cpu_context_t); i++) p[i] = 0;

    driver_pcb.ctx.rip    = entry_point;
    driver_pcb.ctx.rsp    = DRIVER_STACK_TOP;
    driver_pcb.ctx.rflags = 0x202; 

    int i = 0;
    while (name[i] && i < 31) {
        driver_pcb.name[i] = name[i];
        i++;
    }
    driver_pcb.name[i] = '\0';

    return &driver_pcb;
}

void driver_launch(pcb_t* pcb) {
    pcb->state = PROC_RUNNING;
    
    __asm__ volatile (
        "syscall\n\t"
        : 
        : "a" (SYS_THREAD_CREATE),
          "D" (pcb->ctx.rip),
          "S" (0),
          "d" (0)
        : "rcx", "r11", "memory"
    );
}
