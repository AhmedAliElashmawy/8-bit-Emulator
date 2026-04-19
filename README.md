<p align="center">
  <img src="docs/hello.png" alt="Project Demo" width="600">
</p>

# CHIP-8 Emulator

> *Resurrect 1970s programs on your modern machine — pixel by pixel.*

A cycle-accurate CHIP-8 emulator written in C. Feed it a `.ch8` ROM and watch decades-old bytecode come alive through SDL2-rendered graphics. Tetris, Pong, Lunar Lander — all running exactly as they did on hardware that predates most programmers alive today.

---

## Why CHIP-8?

CHIP-8 is the "Hello, World" of emulation. Designed in 1977 for hobbyist microcomputers, it has just 35 opcodes, a 64×32 pixel display, and a hexadecimal keypad. Simple enough to understand completely. Complex enough to feel like real hardware. Building one teaches you how *every* emulator works — from the NES to the PS1.

---

## Features

- Full CHIP-8 instruction set (all 35 opcodes)
- SDL2 rendering and real-time input handling
- Hexadecimal keypad mapped to your keyboard
- Drop-in `.ch8` ROM support — no configuration needed

---

## Project Structure

```
chip8/
├── src/        # C source — CPU, display, input
├── include/    # Header files
├── roms/       # Classic .ch8 ROMs ready to run
├── bin/        # Compiled output
└── Makefile    # One-command build
```

---

## Getting Started

### 1. Install Dependencies

**Linux (Debian/Ubuntu):**
```bash
sudo apt-get install gcc make libsdl2-dev
```

### 2. Build

```bash
make
```

The emulator binary lands in `bin/`.

### 3. Run a ROM

```bash
./bin/chip8 roms/Tetris\ [Fran\ Dachille,\ 1991].ch8
```

Or use the Makefile shortcut to launch the default ROM:

```bash
make run
```

### Clean Up

```bash
make clean
```

---

## Keypad Mapping

CHIP-8 uses a 16-key hexadecimal keypad. It's mapped to your keyboard like this:

```
CHIP-8 Keypad        Your Keyboard
┌───┬───┬───┬───┐    ┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ C │    │ 1 │ 2 │ 3 │ 4 │
├───┼───┼───┼───┤    ├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ D │ →  │ Q │ W │ E │ R │
├───┼───┼───┼───┤    ├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ E │    │ A │ S │ D │ F │
├───┼───┼───┼───┤    ├───┼───┼───┼───┤
│ A │ 0 │ B │ F │    │ Z │ X │ C │ V │
└───┴───┴───┴───┘    └───┴───┴───┴───┘
```

Each ROM uses keys differently — check the original game's documentation to know which keys do what.

---

## Tetris in Action

<p align="center">
  <img src="docs/tetris.gif" alt="Project Demo" width="600">
</p>

---

## Acknowledgements

CHIP-8 was originally designed by Joseph Weisbecker for the COSMAC VIP in 1977. This implementation follows [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM).