add_library(vocoder STATIC vocoder.hpp vocoder.cpp vocoder_types.hpp)
target_include_directories(vocoder PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/util
    ${FFTW3_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
)
