set(target metric_test)

add_executable(${target}
    tests/code_lines_count.cpp
    tests/cyclomatic_complexity.cpp
    tests/parameters_count.cpp
)

target_link_libraries(${target}
    PRIVATE
        GTest::GTest
        GTest::Main
        metric
        function
        file
)

target_compile_definitions(${target} PRIVATE TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/")

add_test(NAME ${target} COMMAND ${target})
