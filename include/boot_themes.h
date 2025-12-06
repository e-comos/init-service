#ifndef BOOT_THEMES_H
#define BOOT_THEMES_H

#include "../include/ecomos_types.h"

// Theme selection
#define THEME_DOTS      0
#define THEME_WHISTLER  1

// Theme functions
void show_dots_animation(void);
void show_whistler_animation(void);
void show_boot_animation_theme(int theme);

#endif