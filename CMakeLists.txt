cmake_minimum_required(VERSION 3.16)
project(chis    VERSION 0.0.1
                DESCRIPTION "Toy CHIP-8 emulator"
                LANGUAGES CXX)
add_executable(chis main.cpp)
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
