#include "virtual_machine.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

void print_opcode_hex(const Opcode opcode) {
    std::cout << std::hex << std::setfill('0') << std::setw(4) << opcode
              << std::dec << std::endl;
}

VirtualMachine::VirtualMachine()
    : rom_size{0}, program_counter{RESERVED_MEMORY_SPACE}, index_register{0},
      delay_timer{0}, sound_timer{0}, memory{}, registers{}, screen{0},
      keypad{0}, stack{}, is_display_redrawn{false} {
    for (size_t i = 0; i < FONT.size(); ++i) {
        this->memory[i + FONT_OFFSET] = FONT[i];
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

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
    this->rom_size = static_cast<size_t>(rom_file_size);
}

void VirtualMachine::dump_memory() {
    for (size_t i = 0; i < RESERVED_MEMORY_SPACE + this->rom_size; ++i) {
        if (i % 20 == 0) {
            std::cout << std::endl;
        }
        std::cout << std::hex << std::setfill('0') << std::setw(2)
                  << std::to_integer<unsigned short>(this->memory[i])
                  << std::dec << " ";
    }
    std::cout << std::endl;
}

void VirtualMachine::set_keypad_value(const size_t index, const uint8_t value) {
    this->keypad[index] = value;
}

void VirtualMachine::run_cycle() {
    if (this->program_counter >= EMULATED_MEMORY_SIZE) {
        return;
    }
    const auto byte_1 = this->memory[this->program_counter];
    const auto byte_2 = this->memory[this->program_counter + 1];
    const Opcode opcode =
        static_cast<Opcode>((std::to_integer<unsigned short>(byte_1) << 8) |
                            std::to_integer<unsigned short>(byte_2));
    this->program_counter += 2;
    // print_opcode_hex(opcode);
    if (opcode == 0x0000) {
        return;
    }
    switch (opcode & 0xF000) {
    case 0xA000: {
        this->index_register = opcode & 0x0FFF;
        break;
    }
    case 0xB000: {
        this->index_register = opcode & 0x0FFF;
        this->index_register += this->registers[0];
        break;
    }
    case 0xC000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_1 = (opcode & 0x0F00) >> 8;
        const auto value = static_cast<uint8_t>(opcode & 0x00FF);
        this->registers[register_index_1] =
            static_cast<uint8_t>((std::rand() % 266) & value);
        break;
    }
    case 0xD000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_x = (opcode & 0x0F00) >> 8;
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_y = (opcode & 0x00F0) >> 4;
        const auto height = static_cast<uint8_t>(opcode & 0x000F);
        const auto x = this->registers[register_index_x];
        const auto y = this->registers[register_index_y];
        this->registers[0xF] = 0;
        for (size_t i = 0; i < height; ++i) {
            const auto pixel =
                static_cast<uint16_t>(this->memory[this->index_register + i]);
            for (size_t j = 0; j < 8; ++j) {
                if ((pixel & (0x80 >> j)) != 0) {
                    if (this->screen[(x + j + ((y + i) * 64))] == 1) {
                        this->registers[0xF] = 1;
                    }
                    this->screen[(x + j + ((y + i) * 64))] ^= 1;
                }
            }
        }
        this->set_is_display_redrawn(true);
        break;
    }
    case 0xE000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_1 = (opcode & 0x0F00) >> 8;
        const auto operation_type = opcode & 0x00FF;
        switch (operation_type) {
        case 0x9E: {
            if (this->keypad[this->registers[register_index_1]] != 0) {
                this->program_counter += 2;
            }
            break;
        }
        case 0xA1: {
            if (this->keypad[this->registers[register_index_1]] == 0) {
                this->program_counter += 2;
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case 0xF000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_1 = (opcode & 0x0F00) >> 8;
        const auto operation_type = opcode & 0x00FF;
        switch (operation_type) {
        case 0x07: {
            this->registers[register_index_1] = this->delay_timer;
            break;
        }
        case 0x15: {
            this->delay_timer = this->registers[register_index_1];
            break;
        }
        case 0x18: {
            this->sound_timer = this->registers[register_index_1];
            break;
        }
        case 0x1E: {
            if (this->index_register + this->registers[register_index_1] >
                0xFFF) {
                this->registers[0xF] = 1;
            }
            this->index_register =
                this->index_register + this->registers[register_index_1];
            break;
        }
        case 0x0A: {
            // TODO: Refactor into more functional style
            bool key_pressed = false;
            for (const auto &key : this->keypad) {
                if (key == 1) {
                    key_pressed = true;
                    this->registers[register_index_1] = key;
                    break;
                }
            }
            if (!key_pressed) {
                this->program_counter -= 2;
            }
            break;
        }
        case 0x29: {
            this->index_register =
                this->registers[register_index_1] * 5 + FONT_OFFSET;
            break;
        }
        case 0x33: {
            auto value =
                static_cast<uint8_t>(this->registers[register_index_1]);
            this->memory[this->index_register + 2] =
                std::byte{static_cast<uint8_t>(value % 10)};
            value /= 10;
            this->memory[this->index_register + 1] =
                std::byte{static_cast<uint8_t>(value % 10)};
            value /= 10;
            this->memory[this->index_register] =
                std::byte{static_cast<uint8_t>(value % 10)};
            break;
        }
        case 0x55: {
            for (size_t i = 0; i <= register_index_1; ++i) {
                this->memory[this->index_register + i] =
                    std::byte{this->registers[i]};
            }
            break;
        }
        case 0x65: {
            for (size_t i = 0; i <= register_index_1; ++i) {
                this->registers[i] = std::to_integer<uint8_t>(
                    this->memory[this->index_register + i]);
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case 0x6000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index = (opcode & 0x0F00) >> 8;
        const auto value = static_cast<uint8_t>(opcode & 0x00FF);
        this->registers[register_index] = value;
        break;
    }
    case 0x7000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index = (opcode & 0x0F00) >> 8;
        const auto value = static_cast<uint8_t>(opcode & 0x00FF);
        this->registers[register_index] += value;
        break;
    }
    case 0x8000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_1 = (opcode & 0x0F00) >> 8;
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_2 = (opcode & 0x00F0) >> 4;
        const auto last_byte = opcode & 0x000F;
        switch (last_byte) {
        case 0x0000: {
            this->registers[register_index_1] =
                this->registers[register_index_2];
            break;
        }
        case 0x0001: {
            this->registers[register_index_1] |=
                this->registers[register_index_2];
            break;
        }
        case 0x0002: {
            this->registers[register_index_1] &=
                this->registers[register_index_2];
            break;
        }
        case 0x0003: {
            this->registers[register_index_1] ^=
                this->registers[register_index_2];
            break;
        }
        case 0x0004: {
            const uint16_t sum = this->registers[register_index_1] +
                                 this->registers[register_index_2];
            if (sum > 255) {
                this->registers[0xF] = 1;
            }
            this->registers[register_index_1] = static_cast<uint8_t>(sum);
            break;
        }
        case 0x0005: {
            if (this->registers[register_index_1] >
                this->registers[register_index_2]) {
                this->registers[0xF] = 1;
            }
            this->registers[register_index_1] =
                this->registers[register_index_1] -
                this->registers[register_index_2];
            break;
        }
        case 0x0006: {
            this->registers[register_index_1] =
                this->registers[register_index_2];
            this->registers[0xF] = this->registers[register_index_1] & 0x1;
            this->registers[register_index_1] >>= 1;
            break;
        }
        case 0x0007: {
            if (this->registers[register_index_2] >
                this->registers[register_index_1]) {
                this->registers[0xF] = 1;
            }
            this->registers[register_index_1] =
                this->registers[register_index_2] -
                this->registers[register_index_1];
            break;
        }
        case 0x000E: {
            this->registers[register_index_1] =
                this->registers[register_index_2];
            this->registers[0xF] = this->registers[register_index_1] & 0x1;
            this->registers[register_index_1] <<= 1;
            break;
        }
        default: {
            break;
        }
        }
        break;
    }
    case 0x3000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index = (opcode & 0x0F00) >> 8;
        const auto value = static_cast<uint8_t>(opcode & 0x00FF);
        if (this->registers[register_index] == value) {
            this->program_counter += 2;
        }
        break;
    }
    case 0x4000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index = (opcode & 0x0F00) >> 8;
        const auto value = static_cast<uint8_t>(opcode & 0x00FF);
        if (this->registers[register_index] != value) {
            this->program_counter += 2;
        }
        break;
    }
    case 0x5000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_1 = (opcode & 0x0F00) >> 8;
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_2 = (opcode & 0x00F0) >> 4;
        if (this->registers[register_index_1] ==
            this->registers[register_index_2]) {
            this->program_counter += 2;
        }
        break;
    }
    case 0x9000: {
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_1 = (opcode & 0x0F00) >> 8;
        const std::array<uint8_t, EMULATED_REGISTER_COUNT>::size_type
            register_index_2 = (opcode & 0x00F0) >> 4;
        if (this->registers[register_index_1] !=
            this->registers[register_index_2]) {
            this->program_counter += 2;
        }
        break;
    }
    case 0x2000: {
        this->stack.push(this->program_counter);
        const auto position = static_cast<size_t>(opcode & 0x0FFF);
        this->program_counter = position;
        break;
    }
    case 0x1000: {
        const auto position = opcode & 0x0FFF;
        this->program_counter = static_cast<size_t>(position);
        break;
    }
    case 0x0000: {
        switch (opcode & 0x000F) {
        case 0x0000: {
            this->set_is_display_redrawn(true);
            for (auto &pixel : this->screen) {
                pixel = 0;
            }
            break;
        }
        case 0x000E: {
            if (this->stack.empty()) {
                break;
            }
            this->program_counter = this->stack.top();
            this->stack.pop();
            break;
        }
        default: {
            std::cout << "Unknown opcode " << opcode << std::endl;
            break;
        }
        }
        break;
    }
    default: {
        std::cout << "Unknown opcode " << opcode << std::endl;
        break;
    }
    }
    if (this->delay_timer > 0) {
        this->delay_timer -= 1;
    }
    if (this->sound_timer > 0) {
        this->sound_timer -= 1;
    }
}
