# OOP - Final project

Mockup garage business POS/DB software made in C++ (OOP) as final assignment for Thomas More OOP course, year 2024-2025.

## Build requirements
- Modern Linux or macOS build environement (min. C++23) (native windows was not tested, CMake config would probably require alterations)
- CMake 3.22 or higher

## Compile project
```bash
cmake -B ./cmake-build-debug -S . -G "Unix Makefiles"
cmake --build ./cmake-build-debug --target all -j256
```

## Run project
```bash
./cmake-build-debug/TM_OOP_Finals
```

## Tested on
- **macOS 15.1.1:** Apple clang-1600.0.26.4 (C++23), CMake 3.29
- **Arch 2024.04.01 (WSL):** GCC 13.3 (C++23), CMake 3.26

## Validated with
### Arch 2024.04.01 (WSL)
- **Valgrind** 3.24.0 
- **ASAN** 1600.0.26.4
- **UBSAN** 1600.0.26.4
- **LSAN** 1600.0.26.4
### macOS 15.1.1
- **ASAN** 13.3
- **UBSAN** 13.3
- **IKOS** v3.4
- **Coverty** 2024.12.0
- **Klocwork** 2024.4
- **Cppcheck** 2.16.0
  

