#ifndef ABSTRACT_DISPLAY_H
#define ABSTRACT_DISPLAY_H

#include <cstdint>

class AbstractDisplay {
  private:
    const uint8_t DISPLAY_WIDTH = 64;
    const uint8_t DISPLAY_HEIGHT = 32;

  public:
    virtual void set_pixel(const uint8_t x, const uint8_t y,
                           const uint8_t n) = 0;
    virtual void clear_screen() = 0;
};
#endif
