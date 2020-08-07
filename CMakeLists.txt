cmake_minimum_required(VERSION 3.16)
project(chis    VERSION 0.0.1
                DESCRIPTION "Toy CHIP-8 emulator"
                LANGUAGES CXX)
find_package(SDL2 REQUIRED)
add_executable(chis ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/virtual_machine.cpp)
target_include_directories(chis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(chis SDL2)
target_compile_features(chis PUBLIC cxx_std_17)
target_compile_options(chis PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:  -Wall
                                                                                            -Werror
                                                                                            -pedantic
                                                                                            -Wextra
                                                                                            -pedantic-errors
                                                                                            -Wconversion
                                                                                            -Wsign-conversion
                                                                                            -Wfatal-errors>
    $<$<CXX_COMPILER_ID:MSVC>: /W4>)
