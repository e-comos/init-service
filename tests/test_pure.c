// Pure C test without any UNIX libraries
#include <stdio.h>

#define CLEAR "\033[2J\033[H"
#define WHITE "\033[37m"
#define BLUE  "\033[34m"
#define CYAN  "\033[36m"
#define RESET "\033[0m"

void pure_delay(int count) {
    for (volatile int i = 0; i < count * 100000; i++);
}

void print_at(int row, int col, const char* text, const char* color) {
    printf("\033[%d;%dH%s%s%s", row, col, color, text, RESET);
}

int main(void) {
    printf(CLEAR);
    
    // Logo
    print_at(8, 37, "E-comOS", WHITE);
    pure_delay(800);
    
    print_at(10, 27, "Microkernel Operating System", CYAN);
    pure_delay(1000);
    
    // Dots in separate area (rows 14-16)
    for (int frame = 0; frame < 40; frame++) {
        // Clear dot area
        print_at(14, 30, "                    ", "");
        print_at(15, 30, "                    ", "");
        print_at(16, 30, "                    ", "");
        
        // Row 1
        printf("\033[14;30H");
        for (int i = 0; i < 10; i++) {
            if (i < (frame + 0) % 13) {
                printf("%s●%s ", BLUE, RESET);
            } else {
                printf("○ ");
            }
        }
        
        // Row 2
        printf("\033[15;30H");
        for (int i = 0; i < 10; i++) {
            if (i < (frame + 3) % 13) {
                printf("%s●%s ", CYAN, RESET);
            } else {
                printf("○ ");
            }
        }
        
        // Row 3
        printf("\033[16;30H");
        for (int i = 0; i < 10; i++) {
            if (i < (frame + 6) % 13) {
                printf("%s●%s ", BLUE, RESET);
            } else {
                printf("○ ");
            }
        }
        
        pure_delay(150);
    }
    
    // Version info
    print_at(19, 35, "Beta 1 Release", CYAN);
    print_at(21, 25, "(c) E-comOS Microkernel Project", WHITE);
    
    pure_delay(2000);
    printf(CLEAR);
    printf("Animation complete!\n");
    return 0;
}