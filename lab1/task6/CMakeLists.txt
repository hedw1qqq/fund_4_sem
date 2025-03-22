add_library(task6_lib
        src/UniquePtr.cpp
        include/UniquePtr.h
)


target_include_directories(task6_lib PUBLIC
        include
)



add_executable(tests6
        tests/UniquePtr_test.cpp
)


target_include_directories(tests6 PRIVATE
        include
        ${googletest_SOURCE_DIR}/include
)

target_compile_options(tests6 PUBLIC ${COMMON_FLAGS})
target_link_options(tests6 PUBLIC ${COMMON_FLAGS})



target_link_libraries(tests6 PRIVATE
        task6_lib
        gtest_main
        gtest

)


add_test(NAME my_unit_tests COMMAND tests6)