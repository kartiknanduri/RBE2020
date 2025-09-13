cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)


project(my_program C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(q8a
   q8a.c
)

target_link_libraries(q8a pico_stdlib)

pico_add_extra_outputs(q8a)
