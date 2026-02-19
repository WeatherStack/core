#!/usr/bin/env bash

# Linux build script for WeatherStack Core (SHELL script)
# -------------------------------------------------------
# Options:
#   build_linux.sh "path/to/build" Debug/Release
# Example:
#   build_linux.sh build Debug

if [ "$2" != "Debug" ] && [ "$2" != "Release"]; then
    echo "$2 must be either Debug or Release"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Creating build path at $1"
    mkdir -p "$1" || { echo "Failed to create directory at $1"; exit 1; }
fi

cd $1

if [ "$2" = "Debug"]; then
    echo "Building with Debug configuration"

    cmake .. -CMAKE_BUILD_TYPE=Debug
    cmake --build . -- -j$(nproc)
else if [ "$2" = "Release" ] then
    echo "Building with Release configuration"

    cmake .. -CMAKE_BUILD_TYPE=Release
    cmake --build . -- -j$(nproc)
fi

cd ..