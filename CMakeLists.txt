cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
set(CXX_STANDARD_REQUIRED CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(effects_app src/main.cpp)

add_library(cmdline STATIC src/cmdline/cmdline.hpp src/cmdline/cmdline.cpp)
target_link_libraries(effects_app cmdline)