#ifndef ECOMOS_TYPES_H
#define ECOMOS_TYPES_H

// Basic type definitions for microkernel
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// System call definitions
#define SYS_IPC_SEND        1
#define SYS_IPC_RECEIVE     2
#define SYS_THREAD_YIELD    3
#define SYS_ADDRESS_MAP     4
#define SYS_IRQ_WAIT        5

// Process types
#define PROCESS_TYPE_KERNEL     0
#define PROCESS_TYPE_SERVICE    1
#define PROCESS_TYPE_DRIVER     2
#define PROCESS_TYPE_USER       3

// Core service IDs
#define SERVICE_MEMORY_MANAGER  1
#define SERVICE_VGA_DISPLAY     2
#define SERVICE_KEYBOARD_INPUT  3
#define SERVICE_FILE_SYSTEM     4
#define SERVICE_SHELL           5

typedef uint32_t process_id_t;
typedef uint32_t thread_id_t;
typedef uint32_t service_id_t;

// IPC message structure
struct ipc_message {
    thread_id_t sender;
    uint32_t size;
    uint8_t data[256];
};

// Service registration structure
struct service {
    service_id_t id;
    process_id_t provider_process;
    char name[32];
    uint32_t capabilities;
};

#endif