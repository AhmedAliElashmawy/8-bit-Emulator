#include "cpu.h"
#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Chip8 chip8;

void init_cpu(Chip8* chip8) {
    // Initialize the CPU state
    chip8->opcode = 0;
    memset(chip8->V, 0, sizeof(chip8->V));
    memset(chip8->memory, 0, sizeof(chip8->memory));
    chip8->PC = 0x200;  // Program counter starts at 0x200
    chip8->I = 0;
    memset(chip8->stack, 0, sizeof(chip8->stack));
    chip8->SP = 0;
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

void fetch(Chip8* chip8) {
    // Fetch the next opcode
    chip8->opcode = (chip8->memory[chip8->PC] << 8) | chip8->memory[chip8->PC + 1];
    fprintf(stdout, "Fetched opcode: 0x%X\n", chip8->opcode);
}

int main() {
    init_cpu(&chip8);
    rom_loader(&chip8, "/home/ali/Desktop/GitProjects/8-bit-Emulator/BMP Viewer - Hello (C8 example) [Hap, 2005].ch8");
    for(;;){
    fetch(&chip8);
    sleep(1); // Sleep for 1 second to slow down the fetch loop
    if (chip8.PC >= MEMORY_SIZE) {
        fprintf(stderr, "Program counter out of bounds\n");
        break;
    }
    decode(&chip8);
    }
    return 0;
}