cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
set(CXX_STANDARD_REQUIRED CXX_STANDARD 20)
add_compile_options(--std=c++20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_executable(effects_app src/main.cpp)

# Utility
# add_library(util STATIC src/util/error_codes.hpp)

# Cmdline
add_library(cmdline STATIC src/cmdline/cmdline.hpp src/cmdline/cmdline.cpp)
target_include_directories(cmdline PUBLIC "src/util")

# Libsndfile
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/")
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

target_link_libraries(effects_app PUBLIC cmdline PkgConfig::SNDFILE)
target_include_directories(effects_app PUBLIC ${SNDFILE_INCLUDE_DIRS} "src/util")