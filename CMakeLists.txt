cmake_minimum_required(VERSION 3.29)
project(TM_OOP_Finals)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)

# ASAN & UBSAN
add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

# Compiler Flags
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Werror")

# Include nlohmann JSON header
include_directories(${CMAKE_SOURCE_DIR}/src/lib/nlohmann)

# Source Files
set(SOURCE_FILES src/main.cpp
        src/object/product/Product.cpp
        src/object/product/Product.h
        src/object/product/tire/Tire.cpp
        src/object/product/tire/Tire.h
        src/object/product/rim/Rim.cpp
        src/object/product/rim/Rim.h
        src/object/customer/Customer.cpp
        src/object/customer/Customer.h
        src/menu/Menu.hpp
        src/menu/ChoiceMenu.hpp
        src/menu/SequentialMenu.hpp
        src/object/user/User.cpp
        src/object/user/User.h
)

# Executable Creation
add_executable(TM_OOP_Finals ${SOURCE_FILES})