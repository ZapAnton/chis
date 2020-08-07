#include "sdl_display.h"
#include <iostream>

SDLDisplay::SDLDisplay() {}

void SDLDisplay::clear_screen() { return; }

void SDLDisplay::set_pixel(const uint8_t x, const uint8_t y, const uint8_t n) {
    std::cout << "Setting: " << x << " " << y << " " << n << std::endl;
}
