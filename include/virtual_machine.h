#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <array>
#include <cstddef>
#include <filesystem>
#include <stack>

using Opcode = unsigned short;
const size_t EMULATED_MEMORY_SIZE = 4096;
const size_t EMULATED_REGISTER_COUNT = 16;
const size_t RESERVED_MEMORY_SPACE = 0x200;

class VirtualMachine {
  private:
    size_t rom_size;
    size_t program_counter;
    unsigned short index_register;
    std::byte delay_timer;
    std::byte sound_timer;
    std::array<std::byte, EMULATED_MEMORY_SIZE> memory;
    std::array<uint8_t, EMULATED_REGISTER_COUNT> registers;
    std::stack<uint16_t> stack;

  public:
    VirtualMachine();
    void load_rom(const std::filesystem::path &rom_file_path);
    void run();
    void dump_memory();
};

#endif
