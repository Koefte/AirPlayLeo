#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define FB_DEVICE "/dev/fb0"
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define BYTES_PER_PIXEL 4 // Assuming 32-bit color depth (RGBA)
#define FONT_WIDTH 8
#define FONT_HEIGHT 8

// Bitmap font for the uppercase letters of the alphabet (ASCII values 65 to 90)
const uint8_t FONT_BITMAP[26][8] = {
    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00}, // 'A'
    {0xFF, 0x80, 0x80, 0x80, 0xFF, 0x80, 0x80, 0xFF}, // 'B'
    {0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, // 'C'
    {0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF}, // 'D'
    {0xFF, 0x81, 0x81, 0x81, 0xFF, 0x81, 0x81, 0x81}, // 'E'
    {0xFF, 0x81, 0x81, 0x81, 0xFF, 0x80, 0x80, 0x80}, // 'F'
    {0xFF, 0x80, 0x80, 0x80, 0x87, 0x80, 0x80, 0xFF}, // 'G'
    {0x81, 0x81, 0x81, 0xFF, 0x81, 0x81, 0x81, 0x81}, // 'H'
    {0xFF, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xFF}, // 'I'
    {0xFF, 0x01, 0x01, 0x01, 0x01, 0x81, 0x81, 0xFE}, // 'J'
    {0x81, 0x82, 0x84, 0xF8, 0x84, 0x82, 0x81, 0x81}, // 'K'
    {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF}, // 'L'
    {0x83, 0x87, 0x8F, 0xFF, 0xFB, 0xF3, 0xE3, 0x83}, // 'M'
    {0x83, 0x87, 0x8F, 0x9B, 0xB3, 0xE3, 0xC3, 0x83}, // 'N'
    {0xFE, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFE}, // 'O'
    {0xFF, 0x81, 0x81, 0x81, 0xFF, 0x80, 0x80, 0x80}, // 'P'
    {0xFE, 0x81, 0x81, 0x81, 0x81, 0x81, 0x83, 0xFE}, // 'Q'
    {0xFF, 0x81, 0x81, 0x81, 0xFF, 0x84, 0x82, 0x81}, // 'R'
    {0x7E, 0x81, 0x80, 0x80, 0x7E, 0x01, 0x01, 0xFE}, // 'S'
    {0xFF, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}, // 'T'
    {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFE}, // 'U'
    {0x81, 0x81, 0x81, 0x81, 0x81, 0x42, 0x24, 0x18}, // 'V'
    {0x81, 0x81, 0x81, 0x81, 0x9B, 0xFF, 0xFF, 0x66}, // 'W'
    {0x81, 0x81, 0x42, 0x24, 0x18, 0x24, 0x42, 0x81}, // 'X'
    {0x81, 0x42, 0x24, 0x18, 0x08, 0x08, 0x08, 0x08}, // 'Y'
    {0xFF, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0xFF}, // 'Z'
};

// Function to draw a single character at position (x, y) with a specified scale
void draw_char(uint32_t *fb_ptr, int x, int y, uint8_t *bitmap, int scale) {
    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            // Check each bit of the bitmap
            if (bitmap[i] & (1 << (7 - j))) {
                // Set pixels to white with specified scale
                for (int k = 0; k < scale; k++) {
                    for (int l = 0; l < scale; l++) {
                        fb_ptr[(y + i * scale + k) * SCREEN_WIDTH + (x + j * scale + l)] = 0xFFFFFFFF;
                    }
                }
            }
        }
    }
}

// Function to draw text at position (x, y) with specified font size and text string
void draw_text(uint32_t *fb_ptr, int x, int y, int font_size, const char *text) {
    int scale = font_size / FONT_HEIGHT; // Calculate the scale based on font size

    // Loop through each character in the text string
    for (int i = 0; i < strlen(text); i++) {
        // Get the ASCII value of the current character
        int ascii_value = (int)text[i];
        
        // Check if the ASCII value is within the range of the uppercase letters
        if (ascii_value >= 65 && ascii_value <= 90) {
            // Calculate the index of the character bitmap in the FONT_BITMAP array
            int bitmap_index = ascii_value - 65; // Assuming only uppercase letters are supported, starting from ASCII 65 ('A')

            // Draw the character at the current position
            draw_char(fb_ptr, x + i * FONT_WIDTH * scale, y, FONT_BITMAP[bitmap_index], scale);
        }
    }
}

int main() {
    int fb_fd;
    uint32_t *fb_ptr;

    // Open the framebuffer device
    fb_fd = open(FB_DEVICE, O_RDWR);
    if (fb_fd == -1) {
        perror("Error opening framebuffer device");
        exit(EXIT_FAILURE);
    }

    // Map the framebuffer into memory
    fb_ptr = (uint32_t *)mmap(NULL, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb_ptr == MAP_FAILED) {
        perror("Error mapping framebuffer");
        close(fb_fd);
        exit(EXIT_FAILURE);
    }

    // Clear the framebuffer (optional)
    memset(fb_ptr, 0, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL);

    // Draw the text "HELLO" at position (100, 100) with a font size of 16
    draw_text(fb_ptr, 100, 100, 16, "HELLO");

    // Wait for a few seconds (optional)
    sleep(5);

    // Unmap the framebuffer
    munmap(fb_ptr, SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL);

    // Close the framebuffer device
    close(fb_fd);

    return 0;
}   
