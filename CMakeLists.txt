cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include(pico_sdk_import.cmake)

project(pico-sound-explorer C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add executable
add_executable(sound_explorer
    src/main.c
    src/waveform_generator.c
    src/adsr_envelope.c
    src/ui_controls.c
    src/uart_comm.c
)

# Create map/bin/hex/uf2 file in addition to ELF
pico_add_extra_outputs(sound_explorer)

# Link libraries
target_link_libraries(sound_explorer
    pico_stdlib
    hardware_pwm
    hardware_adc
    hardware_gpio
    hardware_uart
    hardware_timer
    pico_multicore
)

# Enable USB or UART for stdio
pico_enable_stdio_usb(sound_explorer 1)
pico_enable_stdio_uart(sound_explorer 0)

# Include directories
target_include_directories(sound_explorer PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)