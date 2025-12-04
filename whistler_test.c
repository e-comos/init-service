#include <stdio.h>

#define CLEAR "\033[2J\033[H"
#define WHITE "\033[37m"
#define GREY  "\033[90m"
#define RED   "\033[41m"
#define GREEN "\033[42m"
#define BLUE  "\033[44m"
#define YELLOW "\033[43m"
#define MAGENTA "\033[45m"
#define RESET "\033[0m"

void delay_ms(int ms) {
    for (volatile int i = 0; i < ms * 100000; i++);
}

void print_at(int row, int col, const char* text, const char* color) {
    printf("\033[%d;%dH%s%s%s", row, col, color, text, RESET);
}

void draw_color_bar(int frame) {
    int bar_width = 40;
    int start_col = 20;
    int row = 16;
    
    // Clear bar area
    printf("\033[%d;%dH", row, start_col);
    for (int i = 0; i < bar_width; i++) printf(" ");
    
    // Draw color segments
    printf("\033[%d;%dH", row, start_col);
    
    const char* colors[] = {RED, GREEN, BLUE, YELLOW, MAGENTA};
    int segment_width = 8;
    
    for (int i = 0; i < 5; i++) {
        int progress = (frame - i * 3) % (segment_width + 5);
        printf("%s", colors[i]);
        
        for (int j = 0; j < segment_width && j < progress; j++) {
            printf(" ");
        }
        printf("%s", RESET);
    }
}

int main(void) {
    printf(CLEAR);
    
    // Logo
    print_at(10, 37, "E-comOS", WHITE);
    delay_ms(800);
    
    print_at(12, 27, "Microkernel Operating System", GREY);
    delay_ms(1000);
    
    // Color bar animation
    for (int frame = 0; frame < 60; frame++) {
        draw_color_bar(frame);
        delay_ms(100);
    }
    
    // Version info
    print_at(20, 35, "Beta 1 Release", GREY);
    print_at(22, 25, "(c) E-comOS Microkernel Project", GREY);
    
    delay_ms(2000);
    printf(CLEAR);
    printf("Animation complete!\n");
    return 0;
}