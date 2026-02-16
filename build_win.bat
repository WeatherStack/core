@echo off

REM Windows build script for WeatherStack Core (BATCH script)
REM ---------------------------------------------------------
REM Options:
REM     build_win.bat "path/to/build" Debug/Release "path/to/vcpkg/toolchain/file.cmake"
REM Example:
REM     build_win.bat build Debug "path/to/vcpkg/toolchain/file.cmake"

if not exist %1 (
    echo %1 does not exist, creating...
    mkdir %1
)

if not %2 == Debug if not %2 == Release (
    echo %2 is not a valid option
    exit /B 1
)

cd %1

if %2 == Debug (
    echo Building with Debug configuration

    cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%3"
    cmake --build . --config Debug
) else if %2 == Release (
    echo Building with Release configuration

    cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%3"
    cmake --build . --config Release
)

cd ..