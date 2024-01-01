add_library(resample STATIC resample.hpp resample.cpp)
target_include_directories(resample PUBLIC 
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/math
    ${SNDFILE_INCLUDE_DIRS}
)
