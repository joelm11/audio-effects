cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
set(CXX_STANDARD_REQUIRED CXX_STANDARD 20)
add_compile_options(--std=c++20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_executable(effects_app src/main.cpp)

# Utility
add_library(utils STATIC src/util/util_math.hpp src/util/util_math.cpp)
target_include_directories(utils PUBLIC "src/util/")

# Cmdline
add_library(cmdline STATIC src/cmdline/cmdline.hpp src/cmdline/cmdline.cpp)
target_include_directories(cmdline PUBLIC "src/vocoder" "src/util")
target_link_libraries(cmdline utils)

# Libsndfile
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/")
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

# FFTW
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

# Vocoder
add_library(vocoder STATIC src/vocoder/vocoder.hpp src/vocoder/vocoder.cpp)
target_include_directories(vocoder PUBLIC "src/util" ${SNDFILE_INCLUDE_DIRS} ${FFTW3_INCLUDE_DIRS})
target_link_libraries(vocoder PUBLIC PkgConfig::SNDFILE PkgConfig::FFTW3 utils)

target_link_libraries(effects_app PUBLIC cmdline vocoder)
target_include_directories(effects_app PUBLIC "src/util")