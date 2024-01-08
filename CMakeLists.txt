cmake_minimum_required(VERSION 3.23.2)

project(audio-effects)
add_executable(effects_app src/main.cpp)

set_property(TARGET effects_app PROPERTY CXX_STANDARD 20)
add_compile_options(--std=c++20) # For some reason the above line doesn't do this... Must be set for all files?
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(PkgConfig REQUIRED)
list(APPEND CMAKE_MODULE_PATH "/usr/local/Cellar/") # Lazy

option(DEBUG "Debug for features under development" OFF)
option(TEST  "Build test executable" OFF)

if (DEBUG)
    add_definitions(-DDEBUG)
    set(CMAKE_BUILD_TYPE Debug)
elseif (TEST)
    message(STATUS "Building tests")
    include(CTest)
    enable_testing()
    find_package(Catch2 REQUIRED)
else()
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Libsndfile
pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)

# FFTW
pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)

# Util_Math
add_subdirectory(${PROJECT_SOURCE_DIR}/util/math)
set(UTIL_MATH_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/util/math/inc" CACHE STRING "Path to util_math includes")
set(UTIL_STATUS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/util" CACHE STRING "Path to error header")

# Util_Cmdline
add_subdirectory(${PROJECT_SOURCE_DIR}/util/cmdline)
set(UTIL_CMDLINE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/util/cmdline/inc" CACHE STRING "Path to cmdline includes")

# Vocoder
add_subdirectory(${PROJECT_SOURCE_DIR}/vocoder)
set(VOCODER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/vocoder/inc" CACHE STRING "Path to vocoder includes")

# Resample - Templated header file atm
# add_subdirectory(${PROJECT_SOURCE_DIR}/resample)
set(RESAMPLE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/resample/inc" CACHE STRING "Path to resample includes")

target_include_directories(effects_app PUBLIC 
    ${UTIL_STATUS_INCLUDE_DIR}
    ${RESAMPLE_INCLUDE_DIR}
)

target_link_libraries(effects_app PUBLIC 
    util_math    
    cmdline 
    vocoder 
    # resample  Templated header file atm
    PkgConfig::SNDFILE
    PkgConfig::FFTW3    
)