#include "../include/ecomos_types.h"
#include "../include/syscalls.h"
#include "../include/boot_animation.h"

// Service registry
static struct service services[16];
static int service_count = 0;

// Message types for service management
#define MSG_SERVICE_REGISTER    1
#define MSG_SERVICE_LOOKUP      2
#define MSG_SERVICE_START       3

// Register a service in the registry
static int register_service(service_id_t id, process_id_t process, const char* name) {
    if (service_count >= 16) return -1;
    
    services[service_count].id = id;
    services[service_count].provider_process = process;
    services[service_count].capabilities = 0;
    
    // Copy service name
    int i = 0;
    while (name[i] && i < 31) {
        services[service_count].name[i] = name[i];
        i++;
    }
    services[service_count].name[i] = '\0';
    
    service_count++;
    return 0;
}

// Start core system services
static void start_core_services(void) {
    struct ipc_message msg;
    
    // Start Memory Manager Service (process 1)
    msg.sender = 0; // Init process
    msg.size = 4;
    msg.data[0] = MSG_SERVICE_START;
    msg.data[1] = SERVICE_MEMORY_MANAGER;
    ipc_send(SERVICE_MEMORY_MANAGER, &msg);
    register_service(SERVICE_MEMORY_MANAGER, SERVICE_MEMORY_MANAGER, "memory_manager");
    
    // Start VGA Display Service (process 2)
    msg.data[1] = SERVICE_VGA_DISPLAY;
    ipc_send(SERVICE_VGA_DISPLAY, &msg);
    register_service(SERVICE_VGA_DISPLAY, SERVICE_VGA_DISPLAY, "vga_display");
    
    // Start Keyboard Input Service (process 3)
    msg.data[1] = SERVICE_KEYBOARD_INPUT;
    ipc_send(SERVICE_KEYBOARD_INPUT, &msg);
    register_service(SERVICE_KEYBOARD_INPUT, SERVICE_KEYBOARD_INPUT, "keyboard_input");
    
    // Start File System Service (process 4)
    msg.data[1] = SERVICE_FILE_SYSTEM;
    ipc_send(SERVICE_FILE_SYSTEM, &msg);
    register_service(SERVICE_FILE_SYSTEM, SERVICE_FILE_SYSTEM, "file_system");
    
    // Start Shell Process (process 5)
    msg.data[1] = SERVICE_SHELL;
    ipc_send(SERVICE_SHELL, &msg);
    register_service(SERVICE_SHELL, SERVICE_SHELL, "shell");
}

// Handle service management messages
static void handle_service_message(struct ipc_message* msg) {
    switch (msg->data[0]) {
        case MSG_SERVICE_LOOKUP: {
            service_id_t requested_id = msg->data[1];
            struct ipc_message response;
            response.sender = 0;
            response.size = 8;
            response.data[0] = MSG_SERVICE_LOOKUP;
            
            // Find service
            for (int i = 0; i < service_count; i++) {
                if (services[i].id == requested_id) {
                    *((process_id_t*)&response.data[4]) = services[i].provider_process;
                    ipc_send(msg->sender, &response);
                    return;
                }
            }
            
            // Service not found
            *((process_id_t*)&response.data[4]) = 0;
            ipc_send(msg->sender, &response);
            break;
        }
        
        case MSG_SERVICE_REGISTER: {
            service_id_t id = msg->data[1];
            process_id_t process = msg->sender;
            register_service(id, process, (char*)&msg->data[8]);
            break;
        }
    }
}

// Main init process
int main(void) {
    struct ipc_message msg;
    
    // Show boot animation
    show_boot_animation();
    
    // Initialize core services
    start_core_services();
    
    // Main service loop - handle service registration and lookup
    while (1) {
        if (ipc_receive(&msg) == 0) {
            handle_service_message(&msg);
        }
        thread_yield();
    }
    
    return 0;
}