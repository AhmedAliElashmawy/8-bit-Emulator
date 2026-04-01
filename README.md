# 8-bit CHIP-8 Emulator

A CHIP-8 emulator written in C, utilizing SDL2 for rendering and input handling. This project emulates the original CHIP-8 instruction set, allowing you to play classic retro programs and games like Tetris, Pong, and Lunar Lander.

## Features
- Fully functional CHIP-8 CPU emulation
- Graphics rendering using SDL2
- Hexadecimal keypad input mapping
- Support for classic `.ch8` ROMs

## Project Structure
```text
├── src/      # C source code files
├── include/  # C header files
├── roms/     # CHIP-8 game and program ROMs
├── bin/      # Compiled executables
└── Makefile  # Build instructions
```

## Dependencies
To build and run this emulator, you need a C compiler (like `gcc`) and the SDL2 development libraries.
- **Linux (Debian/Ubuntu):**
  ```bash
  sudo apt-get install gcc make libsdl2-dev
  ```

## Building the Project
To compile the emulator, simply run:
```bash
make
```
This will generate the emulator executable in the `bin/` directory.

## Usage
Run the emulator by passing the path to a CHIP-8 `.ch8` ROM as an argument:
```bash
./bin/chip8 roms/Tetris\ [Fran\ Dachille,\ 1991].ch8
```
Alternatively, you can run the default ROM using the Makefile:
```bash
make run
```

## Cleaning Up
To remove all compiled object files and the generated executable, run:
```bash
make clean
```

