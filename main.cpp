#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Please, provide a path to the CHIP-8 game file" << std::endl;
        return -1;
    }
    std::cout << "Opening game file " << argv[1] << std::endl;
    return 0;
}
