#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include "SDL2/SDL.h"
#include "abstract_display.h"

class SDLDisplay : public AbstractDisplay {
  public:
    SDLDisplay();
    void set_pixel(const uint8_t x, const uint8_t y, const uint8_t n);
    void clear_screen();
};

#endif
