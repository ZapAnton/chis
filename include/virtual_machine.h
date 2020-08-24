#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <abstract_display.h>
#include <array>
#include <cstddef>
#include <filesystem>
#include <stack>

using Opcode = unsigned short;
const size_t EMULATED_MEMORY_SIZE = 4096;
const size_t EMULATED_REGISTER_COUNT = 16;
const size_t RESERVED_MEMORY_SPACE = 0x200;
const size_t FONT_OFFSET = 0x20;
const size_t FONT_ARRAY_SIZE = 80;
const size_t KEYPAD_SIZE = 16;
const std::array<std::byte, FONT_ARRAY_SIZE> FONT{
    std::byte{0xF0}, std::byte{0x90}, std::byte{0x90},
    std::byte{0x90}, std::byte{0xF0}, // 0
    std::byte{0x20}, std::byte{0x60}, std::byte{0x20},
    std::byte{0x20}, std::byte{0x70}, // 1
    std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0},
    std::byte{0x80}, std::byte{0xF0}, // 2
    std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0},
    std::byte{0x10}, std::byte{0xF0}, // 3
    std::byte{0x90}, std::byte{0x90}, std::byte{0xF0},
    std::byte{0x10}, std::byte{0x10}, // 4
    std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0},
    std::byte{0x10}, std::byte{0xF0}, // 5
    std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0},
    std::byte{0x90}, std::byte{0xF0}, // 6
    std::byte{0xF0}, std::byte{0x10}, std::byte{0x20},
    std::byte{0x40}, std::byte{0x40}, // 7
    std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0},
    std::byte{0x90}, std::byte{0xF0}, // 8
    std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0},
    std::byte{0x10}, std::byte{0xF0}, // 9
    std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0},
    std::byte{0x90}, std::byte{0x90}, // A
    std::byte{0xE0}, std::byte{0x90}, std::byte{0xE0},
    std::byte{0x90}, std::byte{0xE0}, // B
    std::byte{0xF0}, std::byte{0x80}, std::byte{0x80},
    std::byte{0x80}, std::byte{0xF0}, // C
    std::byte{0xE0}, std::byte{0x90}, std::byte{0x90},
    std::byte{0x90}, std::byte{0xE0}, // D
    std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0},
    std::byte{0x80}, std::byte{0xF0}, // E
    std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0},
    std::byte{0x80}, std::byte{0x80}, // F
};

class VirtualMachine {
  private:
    size_t rom_size;
    size_t program_counter;
    size_t index_register;
    std::byte delay_timer;
    std::byte sound_timer;
    std::array<std::byte, EMULATED_MEMORY_SIZE> memory;
    std::array<uint8_t, EMULATED_REGISTER_COUNT> registers;
    std::array<uint8_t, 32 * 64> screen;
    std::array<uint8_t, KEYPAD_SIZE> keypad;
    std::stack<uint16_t> stack;
    bool is_display_redrawn;

  public:
    VirtualMachine();
    void load_rom(const std::filesystem::path &rom_file_path);
    void run_cycle();
    void dump_memory();
    void set_is_display_redrawn(const bool is_display_redrawn) {
        this->is_display_redrawn = is_display_redrawn;
    }
    bool get_is_display_redrawn() { return this->is_display_redrawn; }
    std::array<uint8_t, 32 * 64> &get_screen() { return this->screen; }
    void set_keypad_value(const size_t index, const uint8_t value);
};

#endif
