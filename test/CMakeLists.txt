file(GLOB SRCS *.cpp)

include_directories(../src/)

add_executable(testexputils ${SRCS})

target_link_libraries(testexputils
    gtest_main
)

add_test(NAME exputils_test COMMAND testexputils)

# install(TARGETS testfilesystem DESTINATION bin)
