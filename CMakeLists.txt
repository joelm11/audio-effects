cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
add_executable(effects_app src/main.cpp)

set_property(TARGET effects_app PROPERTY CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(--std=c++20)

# Utility
add_subdirectory(${PROJECT_SOURCE_DIR}/util)

# Cmdline
add_subdirectory(${PROJECT_SOURCE_DIR}/cmdline)

# Libsndfile
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/")
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

# FFTW
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

# Vocoder
add_subdirectory(${PROJECT_SOURCE_DIR}/vocoder)

# Resample
add_subdirectory(${PROJECT_SOURCE_DIR}/resample)

target_link_libraries(effects_app PUBLIC 
    util    
    cmdline 
    vocoder 
    resample 
    PkgConfig::SNDFILE
    PkgConfig::FFTW3    
)