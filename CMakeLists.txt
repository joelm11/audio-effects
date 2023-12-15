cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)

add_executable(effects_app src/main.cpp)

add_library(cmdline STATIC src/cmdline/cmdline.hpp src/cmdline/cmdline.cpp)
target_link_libraries(effects_app cmdline)