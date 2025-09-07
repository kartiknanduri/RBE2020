cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)


project(my_program C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(mycode2
   fun.c
   fun
)

target_link_libraries(mycode2 pico_stdlib)

