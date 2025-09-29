cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)


project(my_program C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pwm
   pwm.c
)

target_link_libraries(pwm pico_stdlib hardware_adc hardware_pwm)

pico_add_extra_outputs(pwm)
