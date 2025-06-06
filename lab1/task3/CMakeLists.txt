cmake_minimum_required(VERSION 3.20)
project(LAB1T3)
set(CMAKE_CXX_STANDARD 20)



enable_testing()


set(COMMON_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -fsanitize=address
        -fsanitize=leak
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS
            --coverage
            -fprofile-arcs
            -ftest-coverage
    )
endif()


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Основная библиотека
add_library(my_lib include/container.h
        include/list.h
        src/List.cpp
        include/deque.h
        src/deque.cpp)

target_include_directories(my_lib PUBLIC include)
target_compile_options(my_lib PRIVATE
        ${COMMON_FLAGS}
        $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
)
target_link_options(my_lib PRIVATE
        $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
)


add_executable(tests tests/deque_test.cpp)
target_link_libraries(tests PRIVATE my_lib GTest::gtest_main asan)
target_compile_options(tests PRIVATE
        ${COMMON_FLAGS}
        $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
)
target_link_options(tests PRIVATE
        $<$<CONFIG:Debug>:${COVERAGE_FLAGS}>
)

add_test(NAME MyTests COMMAND tests)

find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/googletest/*' '/usr/include/c++/*' '/gtest/*' --output-file coverage_filtered.info
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS tests
            COMMENT "Генерация отчёта покрытия кода..."
            VERBATIM
    )
else()
    message(WARNING "lcov или genhtml не найдены, цель 'coverage' недоступна.")
endif()

# Цель для cppcheck с исправленным путем
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK}
            --enable=all
            --inconclusive
            --quiet
            --suppress=missingIncludeSystem
            -I include
            src
            tests
            COMMENT "Запуск cppcheck..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
else()
    message(WARNING "cppcheck не найден, цель 'cppcheck' недоступна.")
endif()