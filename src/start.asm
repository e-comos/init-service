; ============================================================================
; init-service entry point - flat binary at 0x400000
; ============================================================================

bits 64
global _start
extern main

section .text
_start:
    cld

    ; Load stack pointer
    mov rsp, stack_top

    ; Jump straight to C code (ensure globals are initialized in .data)
    call main

.hang:
    cli
    hlt
    jmp .hang

section .data
align 16
    times 16384 db 0    ; 16KB stack
stack_top:
