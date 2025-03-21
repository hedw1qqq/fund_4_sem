add_library(task1_lib
        src/array.cpp
        include/Array.h
        include/Container.h
)


target_include_directories(task1_lib PUBLIC
        include
)



add_executable(tests1
        tests/container_test.cpp
)


target_include_directories(tests1 PRIVATE
        include
        ${googletest_SOURCE_DIR}/include
)

target_compile_options(tests1 PUBLIC ${COMMON_FLAGS})
target_link_options(tests1 PUBLIC ${COMMON_FLAGS})



target_link_libraries(tests1 PRIVATE
        task1_lib
        gtest_main
        gtest

)


add_test(NAME my_unit_tests COMMAND tests1)