#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

// Mock VGA buffer for terminal testing
static char screen_buffer[80 * 25];

// ANSI color codes for terminal
#define ANSI_BLACK   "\033[30m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_WHITE   "\033[37m"
#define ANSI_RESET   "\033[0m"
#define ANSI_CLEAR   "\033[2J\033[H"

void clear_screen_test(void) {
    printf(ANSI_CLEAR);
    fflush(stdout);
}

void print_centered_test(const char* text, int row, const char* color) {
    int len = 0;
    const char* p = text;
    while (*p++) len++;
    
    int start_col = (80 - len) / 2;
    printf("\033[%d;%dH%s%s%s", row + 1, start_col + 1, color, text, ANSI_RESET);
    fflush(stdout);
}

void draw_progress_bar_test(int progress, int row) {
    int bar_width = 40;
    int start_col = (80 - bar_width) / 2;
    int filled = (progress * bar_width) / 100;
    
    printf("\033[%d;%dH", row + 1, start_col + 1);
    printf(ANSI_CYAN);
    
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf("█");
        } else {
            printf("░");
        }
    }
    printf(ANSI_RESET);
    fflush(stdout);
}

void delay_test(int ms) {
    usleep(ms * 1000);
}

void draw_progress_dots_test(int frame) {
    int start_col = 30;
    int row = 16;
    
    printf("\033[%d;%dH", row + 1, start_col + 1);
    
    for (int i = 0; i < 20; i++) {
        if (i < frame % 21) {
            printf("%s●%s", ANSI_BLUE, ANSI_RESET);
        } else {
            printf("%s○%s", ANSI_WHITE, ANSI_RESET);
        }
    }
    fflush(stdout);
}

void show_boot_animation_test(void) {
    clear_screen_test();
    
    // Phase 1: Logo fade-in (Whistler style)
    print_centered_test("E-comOS", 10, ANSI_WHITE);
    delay_test(800);
    print_centered_test("Microkernel Operating System", 12, ANSI_CYAN);
    delay_test(1000);
    
    // Phase 2: Animated dots (like Whistler)
    for (int frame = 0; frame < 60; frame++) {
        draw_progress_dots_test(frame);
        delay_test(150);
    }
    
    // Phase 3: Version info
    print_centered_test("Beta 1 Release", 20, ANSI_CYAN);
    print_centered_test("(c) E-comOS Microkernel Project", 22, ANSI_WHITE);
    
    delay_test(2000);
    clear_screen_test();
}

int main(void) {
    printf("E-comOS Boot Animation Test\n");
    printf("Press Enter to start...\n");
    getchar();
    
    show_boot_animation_test();
    
    printf("Animation complete!\n");
    return 0;
}