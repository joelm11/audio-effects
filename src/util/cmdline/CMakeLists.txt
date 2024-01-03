add_library(cmdline STATIC inc/cmdline.hpp src/cmdline.cpp)
target_include_directories(cmdline PUBLIC 
    ${UTIL_CMDLINE_INCLUDE_DIR}
    ${VOCODER_INCLUDE_DIR}
    ${UTIL_STATUS_INCLUDE_DIR}
)