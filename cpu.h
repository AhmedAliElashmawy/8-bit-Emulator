#include <stdint.h>

#define REGISTERS_NUMBER 16
#define MEMORY_SIZE 4096
#define STACK_SIZE 16

typedef struct {
    uint16_t opcode;
    uint8_t V[REGISTERS_NUMBER];
    uint8_t memory[MEMORY_SIZE];
    uint16_t PC;
    uint16_t I;
    uint16_t stack[STACK_SIZE];
    uint8_t SP;
} Chip8;

void init_cpu(Chip8* chip8);

void rom_loader(Chip8* chip8, const char* FilePath);

void fetch(Chip8* chip8);