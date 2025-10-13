#include <stdio.h>


void decode(Chip8* chip8){
    switch(chip8->opcode & 0xF000){
        case 0x000:
            switch (chip8->opcode)
            {
            case 0x00E0: // Not completed yet
                /* Clear Display */
                chip8->PC += 2;
                break;
            
            case 0x00EE:
                chip8->SP -= 1;
                chip8->PC = chip8->stack[chip8->SP];
                chip8->PC += 2;
                break;
            }
        break;

        case 0x1000:
            chip8->PC = chip8->opcode & 0x0FFF;
            break;

        case 0x2000:
            chip8->SP += 1;
            chip8->stack[chip8->SP] = chip8->PC;
            chip8->PC = chip8->opcode & 0x0FFF;
            break;

        case 0x3000:
            if(chip8->V[chip8->opcode & 0x0F00] == (chip8->opcode & 0x00FF)){
                chip8->PC += 4;
            }
            chip8->PC += 2;
            break;

        case 0x4000:
            if(chip8->V[chip8->opcode & 0x0F00] != (chip8->opcode & 0x00FF)){
                chip8->PC += 4;
            }
            chip8->PC += 2;
            break;

        case 0x5000:
            if(chip8->V[chip8->opcode & 0x0F00] == (chip8->opcode & 0x00F0)){
                chip8->PC += 4;
            }
            chip8->PC += 2;
            break;

        case 0x6000:
            chip8->V[chip8->opcode & 0x0F00] = (chip8->opcode & 0x00FF);
            chip8->PC += 2;
            break;

        case 0x7000:
            chip8->V[chip8->opcode & 0x0F00] += (chip8->opcode & 0x00FF);
            chip8->PC += 2;
            break;

        default:
                printf("No opcode yet\n");
                chip8->PC += 2;
        }

}