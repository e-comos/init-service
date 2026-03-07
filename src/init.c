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
    ipc_message_t msg;
    
    // Start Memory Manager Service (process 1)
    msg.type = MSG_SERVICE_START;
    msg.sender_pid = 0;
    msg.receiver_pid = SERVICE_MEMORY_MANAGER;
    msg.data_len = 4;
    msg.data[0] = SERVICE_MEMORY_MANAGER;
    ipc_send_msg_msg(msg.type, 0, msg.receiver_pid, msg.data_len, msg.data);
    register_service(SERVICE_MEMORY_MANAGER, SERVICE_MEMORY_MANAGER, "memory_manager");
    
    // Start VGA Display Service (process 2)
    msg.receiver_pid = SERVICE_VGA_DISPLAY;
    msg.data[0] = SERVICE_VGA_DISPLAY;
    ipc_send_msg_msg(msg.type, 0, msg.receiver_pid, msg.data_len, msg.data);
    register_service(SERVICE_VGA_DISPLAY, SERVICE_VGA_DISPLAY, "vga_display");
    
    // Start Keyboard Input Service (process 3)
    msg.receiver_pid = SERVICE_KEYBOARD_INPUT;
    msg.data[0] = SERVICE_KEYBOARD_INPUT;
    ipc_send_msg_msg(msg.type, 0, msg.receiver_pid, msg.data_len, msg.data);
    register_service(SERVICE_KEYBOARD_INPUT, SERVICE_KEYBOARD_INPUT, "keyboard_input");
    
    // Start File System Service (process 4)
    msg.receiver_pid = SERVICE_FILE_SYSTEM;
    msg.data[0] = SERVICE_FILE_SYSTEM;
    ipc_send_msg_msg(msg.type, 0, msg.receiver_pid, msg.data_len, msg.data);
    register_service(SERVICE_FILE_SYSTEM, SERVICE_FILE_SYSTEM, "file_system");
    
    // Start Shell Service (process 5)
    msg.receiver_pid = SERVICE_SHELL;
    msg.data[0] = SERVICE_SHELL;
    ipc_send_msg_msg(msg.type, 0, msg.receiver_pid, msg.data_len, msg.data);
    register_service(SERVICE_SHELL, SERVICE_SHELL, "shell");
}

// Handle service management messages
static void handle_service_message(ipc_message_t* msg) {
    switch (msg->type) {
        case MSG_SERVICE_LOOKUP: {
            service_id_t requested_id = *((service_id_t*)msg->data);
            
            // Find service
            for (int i = 0; i < service_count; i++) {
                if (services[i].id == requested_id) {
                    process_id_t response_data = services[i].provider_process;
                    ipc_send_msg_msg(MSG_SERVICE_LOOKUP, 0, msg->sender_pid, 
                                sizeof(process_id_t), &response_data);
                    return;
                }
            }
            
            // Service not found
            process_id_t response_data = 0;
            ipc_send_msg_msg(MSG_SERVICE_LOOKUP, 0, msg->sender_pid, 
                        sizeof(process_id_t), &response_data);
            break;
        }
        
        case MSG_SERVICE_REGISTER: {
            service_id_t id = *((service_id_t*)msg->data);
            char* name = (char*)(msg->data + sizeof(service_id_t));
            register_service(id, msg->sender_pid, name);
            break;
        }
    }
}

// Main init process
int main(void) {
    ipc_message_t msg;
    
    // Show boot animation
    show_boot_animation();
    
    // Initialize core services
    start_core_services();
    
    // Main service loop - handle service registration and lookup
    while (1) {
        if (ipc_receive_msg(&msg, 0) == 0) {
            handle_service_message(&msg);
        }
        thread_yield();
    }
    
    return 0;
}