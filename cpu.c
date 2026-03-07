#include "cpu.h"
#include "opcode.h"
#include "sdl_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const uint8_t chip8_fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8 chip8;

// Initialize the CPU state
void init_cpu(Chip8* chip8) {
    memset(chip8->V, 0, sizeof(chip8->V));
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->key, 0, sizeof(chip8->key));
    chip8->opcode = 0;
    chip8->PC = 0x200;  // Program counter starts at 0x200
    chip8->I = 0;
    chip8->SP = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    memcpy(&chip8->memory[FONTSET_START_ADDRESS], chip8_fontset, FONTSET_SIZE);
    clear_screen(chip8);
}


void rom_loader(Chip8* chip8, const char* FilePath) {
    FILE* rom = fopen(FilePath, "rb");
    if (rom == NULL) {
        fprintf(stderr, "Failed to open ROM: %s\n", FilePath);
        exit(EXIT_FAILURE);
    }

    // Get the size of the ROM
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    // Ensure the ROM fits in memory
    if (rom_size > (MEMORY_SIZE - 0x200)) {
        fprintf(stderr, "ROM size exceeds available memory\n");
        fclose(rom);
        exit(EXIT_FAILURE);
    }

    // Load the ROM into memory starting at 0x200
    fread(&chip8->memory[0x200], sizeof(uint8_t), rom_size, rom);
    fprintf(stdout, "ROM loaded successfully: %s (%ld bytes)\n", FilePath, rom_size);
    fclose(rom);
}

// Fetch the next opcode
void fetch(Chip8* chip8) {
    chip8->opcode = (chip8->memory[chip8->PC] << 8) | chip8->memory[chip8->PC + 1];
}

void clear_screen(Chip8* chip8){
    memset(chip8->gfx, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
    chip8->draw_flag = true;
}

void draw_sprite(Chip8* chip8, uint8_t x, uint8_t y, uint8_t height) {
    chip8->V[0xF] = 0;
    for (uint8_t row = 0; row < height; row++) {
        uint8_t sprite_byte = chip8->memory[chip8->I + row];
        for (uint8_t col = 0; col < 8; col++) {
            if ((sprite_byte & (0x80 >> col)) == 0) { continue; }
            uint8_t px = (x + col) % DISPLAY_WIDTH;
            uint8_t py = (y + row) % DISPLAY_HEIGHT;
            if (chip8->gfx[px][py] == 1) { chip8->V[0xF] = 1; }
            chip8->gfx[px][py] ^= 1;
        }
    }
    chip8->draw_flag = true;
}

int main(int argc, char* argv[]) {
    const char* rom_path = (argc > 1) ? argv[1] : "Soccer.ch8";

    init_cpu(&chip8);
    if (!sdl_display_init(PIXEL_SCALE)) {
        return EXIT_FAILURE;
    }
    if (argc <= 1) {
        fprintf(stdout, "No ROM supplied. Defaulting to '%s' in current directory.\n", rom_path);
    }

    rom_loader(&chip8, rom_path);

    bool running = true;
    uint32_t last_render = SDL_GetTicks();

    while (running && chip8.PC < MEMORY_SIZE - 1) {
        if (chip8.PC > MEMORY_SIZE - 2) {
            fprintf(stderr, "Program counter out of bounds: 0x%X\n", chip8.PC);
            break;
        }

        if (!sdl_display_process_events(&chip8)) {
            break;
        }
        sdl_display_update_timers(&chip8);

        fetch(&chip8);
        decode(&chip8);

        uint32_t now = SDL_GetTicks();
        if (now - last_render >= 16) { // ~60Hz
            sdl_display_render(&chip8);
            chip8.draw_flag = false;
            last_render = now;
        }

        sdl_display_delay(1);
    }

    sdl_display_shutdown();
    return 0;
}