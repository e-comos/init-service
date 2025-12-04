#ifndef BOOT_ANIMATION_H
#define BOOT_ANIMATION_H

#include "ecomos_types.h"

// VGA text mode colors
#define COLOR_BLACK     0
#define COLOR_BLUE      1
#define COLOR_GREEN     2
#define COLOR_CYAN      3
#define COLOR_RED       4
#define COLOR_MAGENTA   5
#define COLOR_BROWN     6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW    14
#define COLOR_WHITE     15

// Screen dimensions
#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   25

// Animation functions
void show_boot_animation(void);
void clear_screen(void);
void print_centered(const char* text, int row, uint8_t color);
void draw_progress_bar(int progress, int row);
void delay(uint32_t ms);

#endif