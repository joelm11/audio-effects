cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
set(CXX_STANDARD_REQUIRED CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_executable(effects_app src/main.cpp)

# Cmdline
add_library(cmdline STATIC src/cmdline/cmdline.hpp src/cmdline/cmdline.cpp)

# Libsndfile
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/") # TODO: Change this to system
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

target_link_libraries(effects_app PUBLIC cmdline PkgConfig::SNDFILE)
target_include_directories(effects_app PUBLIC ${SNDFILE_INCLUDE_DIRS})