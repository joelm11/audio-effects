add_library(resample STATIC inc/resample.hpp src/resample.cpp)
include_directories(resample PUBLIC 
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/math/inc
    ${SNDFILE_INCLUDE_DIRS}
)
