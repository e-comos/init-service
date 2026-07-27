#ifndef ECOMOS_TYPES_H
#define ECOMOS_TYPES_H

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef long long          int64_t;
typedef int                int32_t;

// System call numbers
#define SYS_IPC_SEND_MSG    1
#define SYS_IPC_RECEIVE     2
#define SYS_THREAD_YIELD    3
#define SYS_ADDRESS_MAP     4
#define SYS_IRQ_WAIT        5
#define SYS_THREAD_CREATE   8
// Process types
#define PROCESS_TYPE_KERNEL     0
#define PROCESS_TYPE_SERVICE    1
#define PROCESS_TYPE_USER       2

// Core service IDs
#define SERVICE_MEMORY_MANAGER  1
#define SERVICE_VGA_DISPLAY     2
#define SERVICE_KEYBOARD_INPUT  3
#define SERVICE_FILE_SYSTEM     4
#define SERVICE_SHELL           5

// Memory layout
#define INIT_BASE       0x400000ULL
#define EBTS_BASE       0x500000ULL
#define SHARED_BASE     0x600000ULL
#define EBTS_STACK_TOP  (SHARED_BASE + 0x100000ULL)

typedef uint64_t process_id_t;
typedef uint64_t service_id_t;

// Process states
#define PROC_UNUSED     0
#define PROC_READY      1
#define PROC_RUNNING    2
#define PROC_BLOCKED    3
#define PROC_DEAD       4

// CPU context for context switch
typedef struct {
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp, rsp;
    uint64_t r8,  r9,  r10, r11;
    uint64_t r12, r13, r14, r15;
    uint64_t rip, rflags;
} cpu_context_t;

// Process Control Block
typedef struct {
    process_id_t    pid;
    uint8_t         state;
    cpu_context_t   ctx;
    uint64_t        entry;
    uint64_t        stack_top;
    char            name[32];
} pcb_t;

// IPC message
typedef struct {
    uint32_t type;
    uint32_t sender_pid;
    uint32_t receiver_pid;
    uint32_t data_len;
    uint8_t  data[256];
    uint64_t timestamp;
    uint32_t flags;
} ipc_message_t;

// Service registry entry
typedef struct {
    service_id_t    id;
    process_id_t    provider_pid;
    char            name[32];
    uint32_t        capabilities;
} service_t;

int ipc_send_msg_msg(uint32_t type, uint32_t flags, uint32_t receiver_pid,
                     uint32_t data_len, const void* data);

#endif