file(GLOB SRCS *.cpp)

add_executable(testfilesystem ${SRCS})

target_link_libraries(testfilesystem
    gtest_main
    EXP_UTILS
)

add_test(NAME filesystem_test COMMAND testfilesystem)

# install(TARGETS testfilesystem DESTINATION bin)
