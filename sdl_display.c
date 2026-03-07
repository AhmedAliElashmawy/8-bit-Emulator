#include "sdl_display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static uint32_t pixel_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
static int current_pixel_scale = 10;

static void cleanup_and_fail(const char* message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    sdl_display_shutdown();
}

bool sdl_display_init(int pixel_scale) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    if (pixel_scale > 0) {
        current_pixel_scale = pixel_scale;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    window = SDL_CreateWindow(
        "Chip-8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * current_pixel_scale,
        DISPLAY_HEIGHT * current_pixel_scale,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        cleanup_and_fail("SDL_CreateWindow failed");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\nAttempting software renderer...\n", SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == NULL) {
            cleanup_and_fail("SDL_CreateRenderer (software) failed");
            return false;
        }
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT
    );

    if (texture == NULL) {
        cleanup_and_fail("SDL_CreateTexture failed");
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    memset(pixel_buffer, 0, sizeof(pixel_buffer));
    return true;
}

void sdl_display_shutdown(void) {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}

bool sdl_display_process_events(Chip8* chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
            /* Map physical keys to CHIP-8 keypad */
            switch (event.key.keysym.sym) {
                case SDLK_1: chip8->key[0x1] = 1; break;
                case SDLK_2: chip8->key[0x2] = 1; break;
                case SDLK_3: chip8->key[0x3] = 1; break;
                case SDLK_4: chip8->key[0xC] = 1; break;

                case SDLK_q: chip8->key[0x4] = 1; break;
                case SDLK_w: chip8->key[0x5] = 1; break;
                case SDLK_e: chip8->key[0x6] = 1; break;
                case SDLK_r: chip8->key[0xD] = 1; break;

                case SDLK_a: chip8->key[0x7] = 1; break;
                case SDLK_s: chip8->key[0x8] = 1; break;
                case SDLK_d: chip8->key[0x9] = 1; break;
                case SDLK_f: chip8->key[0xE] = 1; break;

                case SDLK_z: chip8->key[0xA] = 1; break;
                case SDLK_x: chip8->key[0x0] = 1; break;
                case SDLK_c: chip8->key[0xB] = 1; break;
                case SDLK_v: chip8->key[0xF] = 1; break;
            }
        }

        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_1: chip8->key[0x1] = 0; break;
                case SDLK_2: chip8->key[0x2] = 0; break;
                case SDLK_3: chip8->key[0x3] = 0; break;
                case SDLK_4: chip8->key[0xC] = 0; break;

                case SDLK_q: chip8->key[0x4] = 0; break;
                case SDLK_w: chip8->key[0x5] = 0; break;
                case SDLK_e: chip8->key[0x6] = 0; break;
                case SDLK_r: chip8->key[0xD] = 0; break;

                case SDLK_a: chip8->key[0x7] = 0; break;
                case SDLK_s: chip8->key[0x8] = 0; break;
                case SDLK_d: chip8->key[0x9] = 0; break;
                case SDLK_f: chip8->key[0xE] = 0; break;

                case SDLK_z: chip8->key[0xA] = 0; break;
                case SDLK_x: chip8->key[0x0] = 0; break;
                case SDLK_c: chip8->key[0xB] = 0; break;
                case SDLK_v: chip8->key[0xF] = 0; break;
            }
        }
    }
    return true;
}

void sdl_display_update_timers(Chip8* chip8) {
    /* Decrement delay_timer and sound_timer at ~60Hz. Use SDL_GetTicks for timing. */
    static uint32_t last_tick = 0;
    uint32_t now = SDL_GetTicks();
    if (last_tick == 0) {
        last_tick = now;
        return;
    }

    /* If 16ms (approx) or more have passed, decrement timers the appropriate number of ticks */
    uint32_t elapsed = now - last_tick;
    if (elapsed >= 16) {
        /* number of 60Hz ticks to apply */
        uint32_t ticks = elapsed / 16;
        for (uint32_t i = 0; i < ticks; ++i) {
            if (chip8->delay_timer > 0) { chip8->delay_timer--; }
            if (chip8->sound_timer > 0) { chip8->sound_timer--; }
        }
        last_tick += ticks * 16;
    }
}

void sdl_display_render(Chip8* chip8) {
    for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
        for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
            uint8_t pixel_on = chip8->gfx[x][y];
            uint32_t color = pixel_on ? 0xFFFFFFFF : 0x00000000;
            pixel_buffer[y * DISPLAY_WIDTH + x] = color;
        }
    }

    if (SDL_UpdateTexture(texture, NULL, pixel_buffer, DISPLAY_WIDTH * sizeof(uint32_t)) != 0) {
        fprintf(stderr, "SDL_UpdateTexture failed: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderClear(renderer);

    SDL_Rect dest_rect = {
        .x = 0,
        .y = 0,
        .w = DISPLAY_WIDTH * current_pixel_scale,
        .h = DISPLAY_HEIGHT * current_pixel_scale
    };

    if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0) {
        fprintf(stderr, "SDL_RenderCopy failed: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
    chip8->draw_flag = false;
}

void sdl_display_delay(uint32_t milliseconds) {
    SDL_Delay(milliseconds);
}
