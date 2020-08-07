#include "SDL2/SDL.h"
#include "virtual_machine.h"
#include <filesystem>
#include <iostream>

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
    if (argc > 2 && std::string(argv[2]) == "--dump") {
        virtual_machine.dump_memory();
        return 0;
    }
    virtual_machine.run();
    return 0;
}
