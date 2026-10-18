cmake_minimum_required(VERSION 3.25)
project(Indecs VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helps on nixos where clangd cant find C headers
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

add_executable(Indecs examples/main.c include/indecs.c)
target_include_directories(Indecs PRIVATE include)
