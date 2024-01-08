add_library(vocoder STATIC inc/vocoder.hpp inc/vocoder_types.hpp src/vocoder.cpp)
target_include_directories(vocoder PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${UTIL_STATUS_INCLUDE_DIR}
    ${UTIL_MATH_INCLUDE_DIR}
    ${UTIL_CMDLINE_INCLUDE_DIR}
    ${PITCH_INCLUDE_DIR}
    ${FFTW3_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
)
# if (TEST)
#     add_executable(test_vocoder test/test_vocoder.cpp)
#     target_include_directories(test_vocoder PUBLIC
#         ${PROJECT_SOURCE_DIR}/vocoder/inc
#     )
#     target_link_libraries(test_vocoder PRIVATE 
#         Catch2::Catch2WithMain
#         vocoder
#         Math
#         PkgConfig::SNDFILE
#         PkgConfig::FFTW3
#     )
#     add_test(NAME test_vocoder COMMAND test_vocoder)
# endif()