#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include "SDL2/SDL.h"
#include "abstract_display.h"

class SDLDisplay : public AbstractDisplay {
  private:
    const uint8_t SCREEN_UPSCALE = 10;

  public:
    SDLDisplay();
    ~SDLDisplay();
    void draw(std::array<uint8_t, 32 * 64> &screen);
    void clear_screen();
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
};

#endif
