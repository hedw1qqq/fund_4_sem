add_library(task5_lib
        src/Vector.cpp
        include/Container.h
        include/Vector.h
)


target_include_directories(task5_lib PUBLIC
        include
)



add_executable(tests5
        tests/vector_test.cpp
)


target_include_directories(tests5 PRIVATE
        include
        ${googletest_SOURCE_DIR}/include
)

target_compile_options(tests5 PUBLIC ${COMMON_FLAGS})
target_link_options(tests5 PUBLIC ${COMMON_FLAGS})



target_link_libraries(tests5 PRIVATE
        task5_lib
        gtest_main
        gtest

)


add_test(NAME my_unit_tests COMMAND tests5)