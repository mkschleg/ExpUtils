file(GLOB SRCS *.cpp)

add_executable(testexputils ${SRCS})

target_link_libraries(testexputils
    gtest_main
    EXP_UTILS
)

add_test(NAME exputils_test COMMAND testexputils)

# install(TARGETS testfilesystem DESTINATION bin)
