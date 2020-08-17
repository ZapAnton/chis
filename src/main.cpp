#include "virtual_machine.h"
#include <filesystem>
#include <iostream>
#include <sdl_display.h>

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
    SDLDisplay sdl_display;
    bool game_running = true;
    while (game_running) {
        // virtual_machine.run_cycle();
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            game_running = false;
            break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                game_running = false;
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
        if (virtual_machine.get_is_display_redrawn()) {
            virtual_machine.set_is_display_redrawn(false);
            sdl_display.draw(virtual_machine.get_screen());
        }
    }
    return 0;
}
