// Pure C animation without UNIX dependencies
// For microkernel environment

// Manual delay without usleep
void pure_delay(unsigned int count) {
    for (volatile unsigned int i = 0; i < count * 50000; i++);
}

// Manual string length
int str_len(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

// VGA text buffer access
static volatile unsigned short* vga = (unsigned short*)0xB8000;

// Colors for VGA
#define VGA_BLACK   0
#define VGA_BLUE    1
#define VGA_GREEN   2
#define VGA_CYAN    3
#define VGA_RED     4
#define VGA_MAGENTA 5
#define VGA_BROWN   6
#define VGA_WHITE   7
#define VGA_GREY    8
#define VGA_LIGHT_BLUE  9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN  11
#define VGA_LIGHT_RED   12
#define VGA_YELLOW      14
#define VGA_BRIGHT_WHITE 15

void clear_vga_screen(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = (VGA_BLACK << 8) | ' ';
    }
}

void print_vga_at(int row, int col, const char* text, unsigned char color) {
    int pos = row * 80 + col;
    for (int i = 0; text[i]; i++) {
        vga[pos + i] = (color << 8) | text[i];
    }
}

void print_vga_centered(const char* text, int row, unsigned char color) {
    int len = str_len(text);
    int col = (80 - len) / 2;
    print_vga_at(row, col, text, color);
}

// Draw progress dots in separate area (not overlapping text)
void draw_vga_dots(int frame) {
    // Clear dot area first
    for (int r = 14; r <= 16; r++) {
        for (int c = 25; c <= 55; c++) {
            vga[r * 80 + c] = (VGA_BLACK << 8) | ' ';
        }
    }
    
    // Draw dots in 3 rows
    int dot_count = 10; // Fewer dots to avoid overlap
    
    // Row 1
    for (int i = 0; i < dot_count; i++) {
        int pos = 14 * 80 + 30 + i * 2; // Space between dots
        if (i < (frame + 0) % (dot_count + 3)) {
            vga[pos] = (VGA_LIGHT_BLUE << 8) | 0xF9; // Dot character
        } else {
            vga[pos] = (VGA_GREY << 8) | 0xFA; // Dim dot
        }
    }
    
    // Row 2
    for (int i = 0; i < dot_count; i++) {
        int pos = 15 * 80 + 30 + i * 2;
        if (i < (frame + 3) % (dot_count + 3)) {
            vga[pos] = (VGA_LIGHT_CYAN << 8) | 0xF9;
        } else {
            vga[pos] = (VGA_GREY << 8) | 0xFA;
        }
    }
    
    // Row 3
    for (int i = 0; i < dot_count; i++) {
        int pos = 16 * 80 + 30 + i * 2;
        if (i < (frame + 6) % (dot_count + 3)) {
            vga[pos] = (VGA_LIGHT_BLUE << 8) | 0xF9;
        } else {
            vga[pos] = (VGA_GREY << 8) | 0xFA;
        }
    }
}

void show_ecomos_animation(void) {
    clear_vga_screen();
    
    // Phase 1: Logo
    print_vga_centered("E-comOS", 8, VGA_BRIGHT_WHITE);
    pure_delay(800);
    
    print_vga_centered("Microkernel Operating System", 10, VGA_LIGHT_CYAN);
    pure_delay(1000);
    
    // Phase 2: Animated dots (separate from text)
    for (int frame = 0; frame < 50; frame++) {
        draw_vga_dots(frame);
        pure_delay(120);
    }
    
    // Phase 3: Version info
    print_vga_centered("Beta 1 Release", 19, VGA_LIGHT_CYAN);
    print_vga_centered("(c) E-comOS Microkernel Project", 21, VGA_WHITE);
    
    pure_delay(2000);
    clear_vga_screen();
}