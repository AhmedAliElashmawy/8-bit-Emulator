#include "sdl_display.h"

#include <SDL2/SDL.h>
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

bool sdl_display_process_events(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            return false;
        }
    }
    return true;
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
