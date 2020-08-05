#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>

const size_t EMULATED_MEMORY_SIZE = 4096;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Please, provide a path to the CHIP-8 game file"
                  << std::endl;
        return -1;
    }
    const char *game_file_path = argv[1];
    std::cout << "Opening game file " << game_file_path << std::endl;
    std::array<std::byte, EMULATED_MEMORY_SIZE> memory = {std::byte{0}};
    std::ifstream game_file{game_file_path, std::ios::in | std::ios::binary};
    if (!game_file.is_open()) {
        std::cerr << "Failed to open game file " << game_file_path << std::endl;
        return -1;
    }
    game_file.read(reinterpret_cast<char *>(&memory.front()), memory.size());
    game_file.close();
    for (size_t i = 0; i < memory.size(); i += 2) {
        const auto byte_1 = memory.at(i);
        const auto byte_2 = memory.at(i + 1);
        const auto opcode = (std::to_integer<unsigned short>(byte_2) << 8) |
                            std::to_integer<unsigned short>(byte_1);
        std::cout << std::hex << opcode << std::endl;
    }
    return 0;
}
