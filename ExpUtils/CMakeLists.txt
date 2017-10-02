

project(ExpUtils)

# add_subdirectory(src)

include_directories(inc/)

file(GLOB EXP_UTILS_SRC src/*.cpp)

add_library(EXP_UTILS ${EXP_UTILS_SRC})
