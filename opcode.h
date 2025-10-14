#ifndef OPCODE_H
#define OPCODE_H

#include <stdio.h>
#include <stdint.h>

#define X(x) ((x & 0x0F00) >> 8)
#define Y(x) ((x & 0x00F0) >> 4)
#define N(x) (x & 0x000F)
#define NN(x) (x & 0x00FF)
#define NNN(x) (x & 0x0FFF)

void decode(Chip8* chip8) {
    uint16_t opcode = chip8->opcode;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0: // Clear display
                    clear_screen(chip8);
                    chip8->PC += 2;
                    break;

                case 0x00EE: // Return from subroutine
                    chip8->SP--;
                    chip8->PC = chip8->stack[chip8->SP];
                    chip8->PC += 2;
                    break;
            }
            break;

        case 0x1000: // JP addr
            chip8->PC = NNN(opcode);
            break;

        case 0x2000: // CALL addr
            chip8->stack[chip8->SP] = chip8->PC;
            chip8->SP++;
            chip8->PC = NNN(opcode);
            break;

        case 0x3000: // SE Vx, byte
            chip8->PC += (chip8->V[X(opcode)] == NN(opcode)) ? 4 : 2;
            break;

        case 0x4000: // SNE Vx, byte
            chip8->PC += (chip8->V[X(opcode)] != NN(opcode)) ? 4 : 2;
            break;

        case 0x5000: // SE Vx, Vy
            chip8->PC += (chip8->V[X(opcode)] == chip8->V[Y(opcode)]) ? 4 : 2;
            break;

        case 0x6000: // LD Vx, byte
            chip8->V[X(opcode)] = NN(opcode);
            chip8->PC += 2;
            break;

        case 0x7000: // ADD Vx, byte
            chip8->V[X(opcode)] += NN(opcode);
            chip8->PC += 2;
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000: // LD Vx, Vy
                    chip8->V[X(opcode)] = chip8->V[Y(opcode)];
                    break;

                case 0x0001: // OR Vx, Vy
                    chip8->V[X(opcode)] |= chip8->V[Y(opcode)];
                    break;

                case 0x0002: // AND Vx, Vy
                    chip8->V[X(opcode)] &= chip8->V[Y(opcode)];
                    break;

                case 0x0003: // XOR Vx, Vy
                    chip8->V[X(opcode)] ^= chip8->V[Y(opcode)];
                    break;

                case 0x0004: { // ADD Vx, Vy with carry
                    int result = chip8->V[X(opcode)] + chip8->V[Y(opcode)];
                    chip8->V[0xF] = (result > 255) ? 1 : 0;
                    chip8->V[X(opcode)] = result & 0xFF;
                    break;
                }

                case 0x0005: { // SUB Vx, Vy
                    uint8_t vx = chip8->V[X(opcode)];
                    uint8_t vy = chip8->V[Y(opcode)];
                    chip8->V[0xF] = (vx > vy) ? 1 : 0;
                    chip8->V[X(opcode)] = (vx - vy) & 0xFF;
                    break;
                }

                case 0x0006: // SHR Vx {, Vy}
                    chip8->V[0xF] = chip8->V[X(opcode)] & 0x01;
                    chip8->V[X(opcode)] >>= 1;
                    break;

                case 0x0007: { // SUBN Vx, Vy
                    uint8_t vx = chip8->V[X(opcode)];
                    uint8_t vy = chip8->V[Y(opcode)];
                    chip8->V[0xF] = (vy > vx) ? 1 : 0;
                    chip8->V[X(opcode)] = (vy - vx) & 0xFF;
                    break;
                }

                case 0x000E: // SHL Vx {, Vy}
                    chip8->V[0xF] = (chip8->V[X(opcode)] & 0x80) >> 7;
                    chip8->V[X(opcode)] <<= 1;
                    break;

                default:
                    printf("Unknown 8XYN opcode: 0x%X\n", opcode);
                    break;
            }
            chip8->PC += 2;
            break;

            case 0xA000: // Set I = nnn
                chip8->I = NNN(opcode);
                chip8->PC += 2;
                break;

            case 0xD000: { // DRW Vx, Vy, nibble
                uint8_t vx = chip8->V[X(opcode)];
                uint8_t vy = chip8->V[Y(opcode)];
                uint8_t height = N(opcode);
                draw_sprite(chip8, vx, vy, height);
                chip8->PC += 2;
                break;
            }

        default:
            printf("Unknown opcode: 0x%X\n", opcode);
            chip8->PC += 2;
            break;
    }
}

#endif