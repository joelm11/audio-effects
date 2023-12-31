add_library(cmdline STATIC cmdline.hpp cmdline.cpp)
target_include_directories(cmdline PUBLIC 
    ${PROJECT_SOURCE_DIR} 
    "${PROJECT_SOURCE_DIR}/vocoder"
)