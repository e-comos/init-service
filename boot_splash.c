#include "boot_animation.h"
#include "syscalls.h"

// Alternative splash screen with ASCII art
void show_ascii_logo(void) {
    clear_screen();
    
    // E-comOS ASCII logo
    print_centered("  _____ ", 6, COLOR_LIGHT_CYAN);
    print_centered(" | ____|", 7, COLOR_LIGHT_CYAN);
    print_centered(" | |__  ", 8, COLOR_LIGHT_CYAN);
    print_centered(" |  __| ", 9, COLOR_LIGHT_CYAN);
    print_centered(" | |___ ", 10, COLOR_LIGHT_CYAN);
    print_centered(" |_____|", 11, COLOR_LIGHT_CYAN);
    
    print_centered("E-comOS", 13, COLOR_WHITE);
    print_centered("Microkernel OS", 14, COLOR_LIGHT_GREY);
    
    delay(2000);
}

// Minimalist boot screen
void show_minimal_boot(void) {
    clear_screen();
    
    // Simple centered logo
    print_centered("E-comOS", 12, COLOR_WHITE);
    
    // Animated dots
    for (int i = 0; i < 3; i++) {
        print_centered("Starting.", 14, COLOR_LIGHT_BLUE);
        delay(300);
        print_centered("Starting..", 14, COLOR_LIGHT_BLUE);
        delay(300);
        print_centered("Starting...", 14, COLOR_LIGHT_BLUE);
        delay(300);
        print_centered("Starting   ", 14, COLOR_LIGHT_BLUE);
        delay(200);
    }
    
    print_centered("Ready", 14, COLOR_LIGHT_GREEN);
    delay(1000);
    clear_screen();
}