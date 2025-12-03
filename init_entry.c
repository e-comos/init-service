// Entry point for init service when called from kernel
// This provides a clean interface between kernel and init service

extern int main(void); // From init.c

// Kernel calls this function to start init service
void init_service_entry(void) {
    main();
}

// Alternative: if kernel needs specific initialization
void init_service_start(void* kernel_info) {
    // Optional: process kernel_info if needed
    main();
}