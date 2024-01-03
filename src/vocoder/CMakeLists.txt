add_library(vocoder STATIC vocoder.hpp vocoder.cpp vocoder_types.hpp pitch.hpp)
target_include_directories(vocoder PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/math
    ${FFTW3_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
)
if (TEST)
    add_executable(test_vocoder test/test_vocoder.cpp)
    target_include_directories(test_vocoder PUBLIC
        ${PROJECT_SOURCE_DIR}/vocoder 
    )
    target_link_libraries(test_vocoder PRIVATE 
        Catch2::Catch2WithMain
        vocoder
        Math
        PkgConfig::SNDFILE
        PkgConfig::FFTW3
    )
    add_test(NAME test_vocoder COMMAND test_vocoder)
endif()