#!/bin/bash

# Raspberry Pi Pico 2 Sound Explorer Build Script
# This script automates the build process for the sound generator

set -e  # Exit on any error

echo "=====================================";
echo "  Pico Sound Explorer Build Script   ";
echo "=====================================";

# Check if PICO_SDK_PATH is set
if [ -z "$PICO_SDK_PATH" ]; then
    echo "Error: PICO_SDK_PATH environment variable is not set"
    echo "Please set it to the path where you installed the Pico SDK"
    echo "Example: export PICO_SDK_PATH=/path/to/pico-sdk"
    exit 1
fi

# Check if the SDK path exists
if [ ! -d "$PICO_SDK_PATH" ]; then
    echo "Error: PICO_SDK_PATH directory does not exist: $PICO_SDK_PATH"
    exit 1
fi

echo "Using Pico SDK at: $PICO_SDK_PATH"

# Check for required tools
if ! command -v cmake &> /dev/null; then
    echo "Error: cmake not found. Please install cmake."
    exit 1
fi

if ! command -v arm-none-eabi-gcc &> /dev/null; then
    echo "Error: arm-none-eabi-gcc not found. Please install the ARM GCC toolchain."
    exit 1
fi

echo "Build tools found successfully"

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring project with CMake..."
cmake ..

# Build the project
echo "Building project..."
make -j$(nproc)

# Check if build was successful
if [ -f "sound_explorer.uf2" ]; then
    echo ""
    echo "=====================================";
    echo "       BUILD SUCCESSFUL!            ";
    echo "=====================================";
    echo ""
    echo "The sound_explorer.uf2 file has been created in the build directory."
    echo ""
    echo "To flash to your Pico:"
    echo "1. Hold the BOOTSEL button while plugging in the Pico"
    echo "2. Copy sound_explorer.uf2 to the RPI-RP2 drive"
    echo "3. The Pico will automatically reboot and run the program"
    echo ""
    echo "Connect to the USB serial port to see status messages!"
else
    echo "Error: Build failed - sound_explorer.uf2 not found"
    exit 1
fi