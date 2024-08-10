set(project dht11_c)
cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(${project} LANGUAGES C CXX ASM)
pico_sdk_init()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(${project} demo.c src/dht.c)
target_link_libraries(${project} pico_stdlib pico_cyw43_arch_none)

pico_add_extra_outputs(${project})
pico_enable_stdio_usb(${project} 1)
pico_enable_stdio_uart(${project} 0)
