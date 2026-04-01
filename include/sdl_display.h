#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "cpu.h"

bool sdl_display_init(int pixel_scale);
void sdl_display_shutdown(void);
bool sdl_display_process_events(Chip8* chip8);
void sdl_display_render(Chip8* chip8);
void sdl_display_delay(uint32_t milliseconds);
void sdl_display_update_timers(Chip8* chip8);

#endif
