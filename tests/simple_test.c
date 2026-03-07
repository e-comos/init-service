#include <stdio.h>
#include <unistd.h>

#define ANSI_CLEAR   "\033[2J\033[H"
#define ANSI_WHITE   "\033[37m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_RESET   "\033[0m"

void clear_screen(void) {
    printf(ANSI_CLEAR);
}

void print_at(int row, int col, const char* text, const char* color) {
    printf("\033[%d;%dH%s%s%s", row, col, color, text, ANSI_RESET);
    fflush(stdout);
}

int main(void) {
    clear_screen();
    
    // E-comOS logo
    print_at(10, 37, "E-comOS", ANSI_WHITE);
    usleep(800000);
    
    print_at(12, 30, "Microkernel Operating System", ANSI_CYAN);
    usleep(1000000);
    
    // Animated dots (multiple rows like Whistler)
    for (int frame = 0; frame < 80; frame++) {
        // Row 1
        printf("\033[15;30H");
        for (int i = 0; i < 20; i++) {
            if (i < (frame + 0) % 25) {
                printf("%s●%s", ANSI_BLUE, ANSI_RESET);
            } else {
                printf("%s○%s", ANSI_WHITE, ANSI_RESET);
            }
        }
        
        // Row 2
        printf("\033[16;30H");
        for (int i = 0; i < 20; i++) {
            if (i < (frame + 5) % 25) {
                printf("%s●%s", ANSI_CYAN, ANSI_RESET);
            } else {
                printf("%s○%s", ANSI_WHITE, ANSI_RESET);
            }
        }
        
        // Row 3
        printf("\033[17;30H");
        for (int i = 0; i < 20; i++) {
            if (i < (frame + 10) % 25) {
                printf("%s●%s", ANSI_BLUE, ANSI_RESET);
            } else {
                printf("%s○%s", ANSI_WHITE, ANSI_RESET);
            }
        }
        
        fflush(stdout);
        usleep(120000);
    }
    
    // Version info
    print_at(21, 35, "Version 0.0.1", ANSI_CYAN);
    print_at(23, 25, "Copyright (C) 2025,2026 E-comOS Operation System Project", ANSI_WHITE);
    
    usleep(2000000);
    clear_screen();
    
    printf("Animation complete!\n");
    return 0;
}