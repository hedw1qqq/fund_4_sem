add_library(task2_lib
        src/list.cpp
        include/Container.h
)


target_include_directories(task2_lib PUBLIC
        include
)



add_executable(tests2
        tests/list_test.cpp
)


target_include_directories(tests2 PRIVATE
        include
        ${googletest_SOURCE_DIR}/include
)

target_compile_options(tests2 PUBLIC ${COMMON_FLAGS})
target_link_options(tests2 PUBLIC ${COMMON_FLAGS})



target_link_libraries(tests2 PRIVATE
        task2_lib
        gtest_main
        gtest

)


add_test(NAME my_unit_tests COMMAND tests2)