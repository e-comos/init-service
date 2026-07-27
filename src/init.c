#include "../include/ecomos_types.h"
#include "../include/syscalls.h"
#include "../include/ebts_loader.h"
#include "../include/boot_animation.h"

#define MSG_SERVICE_REGISTER    1
#define MSG_SERVICE_LOOKUP      2
#define MSG_SERVICE_START       3

typedef struct {
    uint64_t ebts_src;   
    uint64_t ebts_size;  
    uint32_t flags;
} __attribute__((packed)) boot_info_t;

static service_t services[16];
static int service_count = 0;

static int register_service(service_id_t id, process_id_t pid, const char* name) {
    if (service_count >= 16) return -1;
    service_t* s = &services[service_count++];
    s->id           = id;
    s->provider_pid = pid;
    s->capabilities = 0;
    int i = 0;
    while (name[i] && i < 31) { s->name[i] = name[i]; i++; }
    s->name[i] = '\0';
    return 0;
}

static void start_core_services(void) {
    static const struct { service_id_t id; const char* name; } core[] = {
        { SERVICE_MEMORY_MANAGER,  "memory_manager"  },
        { SERVICE_VGA_DISPLAY,     "vga_display"     },
        { SERVICE_KEYBOARD_INPUT,  "keyboard_input"  },
        { SERVICE_FILE_SYSTEM,     "file_system"     },
    };

    for (int i = 0; i < 4; i++) {
        uint32_t id32 = (uint32_t)core[i].id;
        ipc_send_msg_msg(MSG_SERVICE_START, 0, (uint32_t)core[i].id,
                         sizeof(id32), &id32);
        register_service(core[i].id, core[i].id, core[i].name);
    }
}

static void handle_service_message(ipc_message_t* msg) {
    switch (msg->type) {
        case MSG_SERVICE_LOOKUP: {
            service_id_t req = *((service_id_t*)msg->data);
            process_id_t resp = 0;
            for (int i = 0; i < service_count; i++) {
                if (services[i].id == req) { resp = services[i].provider_pid; break; }
            }
            ipc_send_msg_msg(MSG_SERVICE_LOOKUP, 0, msg->sender_pid,
                             sizeof(resp), &resp);
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

int main(void) {
    show_boot_animation();

    start_core_services();

    boot_info_t* binfo = (boot_info_t*)SHARED_BASE;
    if (binfo->ebts_src && binfo->ebts_size) {
        if (ebts_load(binfo->ebts_src, binfo->ebts_size) == 0) {
            register_service(SERVICE_SHELL, SERVICE_SHELL, "ebts_shell");
            pcb_t* pcb = ebts_prepare_pcb();
            ebts_launch(pcb); 
        }
    }

    ipc_message_t msg;
    while (1) {
        if (ipc_receive_msg(&msg, 0) == 0)
            handle_service_message(&msg);
        thread_yield();
    }

    return 0;
}
