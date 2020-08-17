#ifndef ABSTRACT_DISPLAY_H
#define ABSTRACT_DISPLAY_H

#include <array>
#include <cstdint>

class AbstractDisplay {
  protected:
    const uint8_t DISPLAY_WIDTH = 64;
    const uint8_t DISPLAY_HEIGHT = 32;

  public:
    virtual void draw(std::array<uint8_t, 32 * 64> &scren) = 0;
    virtual void clear_screen() = 0;
};
#endif
