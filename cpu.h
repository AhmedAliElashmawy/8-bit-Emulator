#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define REGISTERS_NUMBER 16
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define FONTSET_START_ADDRESS 0x50
#define FONTSET_SIZE 80
#define PIXEL_SCALE 10

typedef struct {
    uint16_t opcode;
    uint8_t V[REGISTERS_NUMBER];
    uint8_t memory[MEMORY_SIZE];
    uint16_t PC;
    uint16_t I;
    uint16_t stack[STACK_SIZE];
    uint8_t SP;
    uint8_t gfx[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    bool draw_flag;
} Chip8;

void init_cpu(Chip8* chip8);

void rom_loader(Chip8* chip8, const char* FilePath);

void fetch(Chip8* chip8);

void clear_screen(Chip8* chip8);
void draw_sprite(Chip8* chip8, uint8_t x, uint8_t y, uint8_t height);

#endif