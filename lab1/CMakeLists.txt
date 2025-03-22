cmake_minimum_required(VERSION 3.22)
project(lab1)
set (CMAKE_CXX_STANDARD 20)
enable_testing()

# Устанавливаем флаги компиляции
set(COMMON_FLAGS -Wall -Wextra -Werror -Wpedantic -fsanitize=address -fsanitize=leak)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COVERAGE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
endif()

# Подключаем GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Добавляем поддиректорию task1
add_subdirectory(task1)
add_subdirectory(task2)
add_subdirectory(task5)
add_subdirectory(task6)
# Проверяем наличие инструментов покрытия кода
find_program(LCOV lcov)
find_program(GENHTML genhtml)

if(LCOV AND GENHTML)
    add_custom_target(coverage
            COMMAND ${LCOV} --capture --directory . --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/googletest/*' --output-file coverage_filtered.info
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Генерация отчёта покрытия кода..."
            VERBATIM
    )
else()
    message(WARNING "lcov или genhtml не найдены, цель 'coverage' недоступна.")
endif()

# Проверяем наличие cppcheck
find_program(CPPCHECK cppcheck)

if(CPPCHECK)
    add_custom_target(cppcheck
            COMMAND ${CPPCHECK} --enable=all --inconclusive --quiet --suppress=missingIncludeSystem -I task1/include task1/src task1/test
            COMMENT "Запуск cppcheck..."
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            VERBATIM
    )
else()
    message(WARNING "cppcheck не найден, цель 'cppcheck' недоступна.")
endif()
