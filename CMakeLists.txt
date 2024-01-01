cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
add_executable(effects_app src/main.cpp)

set_property(TARGET effects_app PROPERTY CXX_STANDARD 20)
add_compile_options(--std=c++20) # For some reason the above line doesn't do this... Must be set for all files?
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

option(DEBUG "Debug for features under development" OFF)

if (DEBUG)
    add_definitions(-DDEBUG)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Libsndfile
find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/")
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

# FFTW
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

# Math
add_subdirectory(${PROJECT_SOURCE_DIR}/math)

# Cmdline
add_subdirectory(${PROJECT_SOURCE_DIR}/cmdline)

# Vocoder
add_subdirectory(${PROJECT_SOURCE_DIR}/vocoder)

# Resample
add_subdirectory(${PROJECT_SOURCE_DIR}/resample)

target_link_libraries(effects_app PUBLIC 
    Math    
    cmdline 
    vocoder 
    resample 
    PkgConfig::SNDFILE
    PkgConfig::FFTW3    
)