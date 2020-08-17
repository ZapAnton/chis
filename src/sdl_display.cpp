#include "sdl_display.h"
#include <iostream>

SDLDisplay::SDLDisplay() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
                  << std::endl;
        SDL_Quit();
        return;
    }
    this->window = SDL_CreateWindow(
        "Chis", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        this->DISPLAY_WIDTH * this->SCREEN_UPSCALE,
        this->DISPLAY_HEIGHT * this->SCREEN_UPSCALE, SDL_WINDOW_SHOWN);
    if (this->window == nullptr) {
        std::cerr << "SDL could not create window! SDL_Error: "
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    SDL_RenderSetLogicalSize(this->renderer,
                             this->DISPLAY_WIDTH * this->SCREEN_UPSCALE,
                             this->DISPLAY_HEIGHT * this->SCREEN_UPSCALE);
    this->texture = SDL_CreateTexture(
        this->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        this->DISPLAY_WIDTH, this->DISPLAY_HEIGHT);
    if (this->texture == nullptr) {
        std::cerr << "SDL could not create texture! SDL_Error: "
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
}

SDLDisplay::~SDLDisplay() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void SDLDisplay::clear_screen() { return; }

void SDLDisplay::draw(std::array<uint8_t, 32 * 64> &screen) {
    std::array<uint32_t, 32 * 64> pixels{0};
    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i] = (screen[i] == 0) ? 0xFF000000 : 0xFFFFFFFF;
    }
    SDL_UpdateTexture(this->texture, NULL, &pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(this->renderer);
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    SDL_RenderPresent(this->renderer);
}
