#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>

const size_t EMULATED_MEMORY_SIZE = 4096;
const size_t EMULATED_REGISTER_COUNT = 16;
const size_t RESERVED_MEMORY_SPACE = 512;
using Opcode = unsigned short;

class VirtualMachine {
  private:
    unsigned short program_counter;
    unsigned short index_register;
    std::byte delay_timer;
    std::byte sound_timer;
    std::array<std::byte, EMULATED_MEMORY_SIZE> memory;
    std::array<std::byte, EMULATED_REGISTER_COUNT> registers;
    std::stack<unsigned short> stack;

  public:
    VirtualMachine();
    void load_rom(const std::filesystem::path &rom_file_path);
};

VirtualMachine::VirtualMachine()
    : program_counter{0}, index_register{0}, delay_timer{std::byte{0}},
      sound_timer{std::byte{0}}, memory{}, registers{}, stack{} {}

void VirtualMachine::load_rom(const std::filesystem::path &rom_file_path) {
    // TODO: Add error handling
    std::ifstream rom_file{rom_file_path, std::ios::binary};
    rom_file.unsetf(std::ios::skipws);
    rom_file.seekg(0, std::ios::end);
    const auto rom_file_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);
    std::vector<std::byte> temp_buffer(
        static_cast<std::vector<std::byte>::size_type>(rom_file_size));
    rom_file.read(reinterpret_cast<char *>(&temp_buffer.front()),
                  rom_file_size);
    for (size_t i = 0; i < temp_buffer.size(); ++i) {
        this->memory[i + RESERVED_MEMORY_SPACE] = temp_buffer.at(i);
    }
}

void print_opcode_hex(const Opcode opcode) {
    std::cout << std::hex << std::setfill('0') << std::setw(4) << opcode
              << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Please, provide a path to the CHIP-8 game file"
                  << std::endl;
        return -1;
    }
    VirtualMachine virtual_machine;
    const std::filesystem::path rom_file_path{argv[1]};
    if (!std::filesystem::exists(rom_file_path)) {
        std::cerr << "Provided path does not exist: " << rom_file_path
                  << std::endl;
        return -1;
    }
    std::cout << "Opening ROM file " << rom_file_path << std::endl;
    virtual_machine.load_rom(rom_file_path);
    return 0;
}
