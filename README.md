# OOP - Final project

Mockup garage business POS/DB software made in C++ (OOP) as final assignment for Thomas More practice enterprise course, year 2024-2025.

## Build requirements
- Linux or macOS build environement (windows was not tested, CMake config would probably require alterations)
- CMake 3.26 or higher

## Compile project
```bash
cmake -B ./cmake-build-debug -S . -G "Unix Makefiles"
cmake --build ./cmake-build-debug --target all -j256
```

## Run project
```bash
./cmake-build-debug/chess
```

## Tested on
- **macOS 16.0.0:** Apple clang-1600.0.26.4 (C++23), CMake 3.29
- **Arch 2024.04.01:** GCC 13.3 (C++23), CMake 3.26
