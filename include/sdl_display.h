#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include "SDL2/SDL.h"
#include "abstract_display.h"
#include <optional>

class SDLDisplay : public AbstractDisplay {
  private:
    const uint8_t SCREEN_UPSCALE = 20;
    const std::array<SDL_Keycode, 16> keypad{
        SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_q, SDLK_w, SDLK_e, SDLK_r,
        SDLK_a, SDLK_s, SDLK_d, SDLK_f, SDLK_z, SDLK_x, SDLK_c, SDLK_v,
    };

  public:
    SDLDisplay();
    ~SDLDisplay();
    void draw(std::array<uint8_t, 32 * 64> &screen);
    void clear_screen();
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;
    std::optional<size_t> get_key_index(const SDL_Keycode keycode);
};

#endif
