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
target_include_directories(cmdline PUBLIC "src/util" PUBLIC "src/vocoder")

# Libsndfile
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/")
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

# FFTW
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

# Vocoder
add_library(vocoder STATIC src/vocoder/vocoder.hpp src/vocoder/vocoder.cpp)
target_include_directories(vocoder PUBLIC "src/util" PUBLIC ${SNDFILE_INCLUDE_DIRS} PUBLIC ${FFTW3_INCLUDE_DIRS})
target_link_libraries(vocoder PUBLIC PkgConfig::SNDFILE PUBLIC PkgConfig::FFTW3)

target_link_libraries(effects_app PUBLIC cmdline vocoder)
target_include_directories(effects_app PUBLIC "src/util")