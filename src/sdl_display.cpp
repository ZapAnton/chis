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
    SDL_RenderClear(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->renderer);
}

SDLDisplay::~SDLDisplay() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void SDLDisplay::clear_screen() { return; }

void SDLDisplay::draw(std::array<uint8_t, 32 * 64> &screen) {
    std::cout << screen[0] << std::endl;
}
