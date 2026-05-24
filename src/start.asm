; ============================================================================
; init-service entry point - must be first in flat binary at 0x400000
; 64-bit long mode, kernel has already set up GDT/paging before jumping here
; ============================================================================

bits 64
global _start
extern main
extern __bss_start
extern __bss_end

section .text
_start:
    cld

    ; Load stack pointer from linker-defined symbol
    mov rsp, stack_top

    ; Zero BSS
    mov rdi, __bss_start
    mov rcx, __bss_end
    sub rcx, rdi
    test rcx, rcx
    jz  .bss_done
    rep stosb
.bss_done:

    call main

.hang:
    cli
    hlt
    jmp .hang

; Stack in .data so it's included in the flat binary
section .data
align 16
    times 16384 db 0    ; 16KB stack
stack_top:
