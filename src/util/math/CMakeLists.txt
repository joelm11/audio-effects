add_library(util_math STATIC inc/util_math.hpp src/util_math.cpp)
target_include_directories(util_math PUBLIC 
    ${UTIL_MATH_INCLUDE_DIR}
)
