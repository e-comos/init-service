#include "../include/boot_animation.h"
#include "../include/boot_themes.h"
#include "../include/syscalls.h"

// VGA text buffer
static volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

// Manual delay without system calls
void manual_delay(uint32_t count) {
    for (volatile uint32_t i = 0; i < count * 50000; i++);
}

// String length helper
int string_length(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

// Clear screen with black background
void clear_screen(void) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga_buffer[i] = (COLOR_BLACK << 8) | ' ';
    }
}

// Print text centered on screen
void print_centered(const char* text, int row, uint8_t color) {
    int len = string_length(text);
    int start_col = (SCREEN_WIDTH - len) / 2;
    int pos = row * SCREEN_WIDTH + start_col;
    
    for (int i = 0; text[i]; i++) {
        vga_buffer[pos + i] = (color << 8) | text[i];
    }
}

// Simple delay function without system calls
void delay(uint32_t ms) {
    manual_delay(ms);
}

// Draw animated progress dots
void draw_progress_dots(int frame) {
    // Clear dot area first
    for (int r = 14; r <= 16; r++) {
        for (int c = 25; c <= 55; c++) {
            vga_buffer[r * SCREEN_WIDTH + c] = (COLOR_BLACK << 8) | ' ';
        }
    }
    
    int dot_count = 10;
    
    // Row 1
    for (int i = 0; i < dot_count; i++) {
        int pos = 14 * SCREEN_WIDTH + 30 + i * 2;
        if (i < (frame + 0) % (dot_count + 3)) {
            vga_buffer[pos] = (COLOR_LIGHT_BLUE << 8) | 0xF9;
        } else {
            vga_buffer[pos] = (COLOR_DARK_GREY << 8) | 0xFA;
        }
    }
    
    // Row 2
    for (int i = 0; i < dot_count; i++) {
        int pos = 15 * SCREEN_WIDTH + 30 + i * 2;
        if (i < (frame + 3) % (dot_count + 3)) {
            vga_buffer[pos] = (COLOR_LIGHT_CYAN << 8) | 0xF9;
        } else {
            vga_buffer[pos] = (COLOR_DARK_GREY << 8) | 0xFA;
        }
    }
    
    // Row 3
    for (int i = 0; i < dot_count; i++) {
        int pos = 16 * SCREEN_WIDTH + 30 + i * 2;
        if (i < (frame + 6) % (dot_count + 3)) {
            vga_buffer[pos] = (COLOR_LIGHT_BLUE << 8) | 0xF9;
        } else {
            vga_buffer[pos] = (COLOR_DARK_GREY << 8) | 0xFA;
        }
    }
}

// Draw Whistler-style color bar
void draw_color_bar(int frame) {
    int bar_width = 20;
    int start_col = (SCREEN_WIDTH - bar_width) / 2;
    int row = 16;
    
    // Clear bar area
    for (int i = 0; i < bar_width; i++) {
        vga_buffer[row * SCREEN_WIDTH + start_col + i] = (COLOR_BLACK << 8) | ' ';
    }
    
    // Draw animated color segments
    int segment_width = 4;
    int colors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_MAGENTA};
    
    for (int i = 0; i < 5; i++) {
        int seg_start = start_col + i * segment_width;
        int progress = (frame - i * 2) % (segment_width + 3);
        
        for (int j = 0; j < segment_width && j < progress; j++) {
            if (seg_start + j < start_col + bar_width) {
                vga_buffer[row * SCREEN_WIDTH + seg_start + j] = (colors[i] << 8) | 0xDB;
            }
        }
    }
}

// Dots theme animation
void show_dots_animation(void) {
    clear_screen();
    
    print_centered("E-comOS", 8, COLOR_WHITE);
    delay(800);
    print_centered("Microkernel Operating System", 10, COLOR_LIGHT_GREY);
    delay(1000);
    
    for (int frame = 0; frame < 50; frame++) {
        draw_progress_dots(frame);
        delay(120);
    }
    
    print_centered("Beta 1 Release", 19, COLOR_LIGHT_GREY);
    print_centered("(c) E-comOS Microkernel Project", 21, COLOR_DARK_GREY);
    
    delay(2000);
    clear_screen();
}

// Whistler theme animation
void show_whistler_animation(void) {
    clear_screen();
    
    print_centered("E-comOS", 10, COLOR_WHITE);
    delay(800);
    print_centered("Microkernel Operating System", 12, COLOR_LIGHT_GREY);
    delay(1000);
    
    for (int frame = 0; frame < 60; frame++) {
        draw_color_bar(frame);
        delay(100);
    }
    
    print_centered("Version 0.0.1", 20, COLOR_LIGHT_GREY);
    print_centered("(c) Saladin5101", 22, COLOR_DARK_GREY);
    
    delay(2000);
    clear_screen();
}

// Theme selector
void show_boot_animation_theme(int theme) {
    if (theme == THEME_WHISTLER) {
        show_whistler_animation();
    } else {
        show_dots_animation();
    }
}

// Default animation (backwards compatibility)
void show_boot_animation(void) {
    show_boot_animation_theme(THEME_WHISTLER);
}