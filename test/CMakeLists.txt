include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
    set(OPTIONS /W4 /WX)
    check_cxx_compiler_flag(/permissive- HAS_PERMISSIVE_FLAG)
    if(HAS_PERMISSIVE_FLAG)
        set(OPTIONS ${OPTIONS} /permissive-)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(OPTIONS -Wall -Wextra -pedantic-errors -Werror)
endif()

function(make_test src target std)
    add_executable(${target} ${src})
    target_compile_options(${target} PRIVATE ${OPTIONS})
    target_include_directories(${target} PRIVATE 3rdparty)
    target_compile_definitions(${target} PRIVATE DOCTEST_CONFIG_USE_STD_HEADERS FMT_HEADER_ONLY)
    if(NAMEOF_TEST_REQUIRE_FMT AND src STREQUAL "test.cpp")
        target_compile_definitions(${target} PRIVATE NAMEOF_TEST_REQUIRE_FMT)
    endif()
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    if(std)
        target_compile_features(${target} PRIVATE cxx_std_${std})
    endif()
    add_test(NAME ${target} COMMAND ${target})
endfunction()

function(make_compile_fail_test case_name case_id expected_text)
    set(target "compile-fail-${case_name}")
    set(unexpected_text "${ARGV3}")

    add_executable(${target} EXCLUDE_FROM_ALL compile_fail.cpp)
    target_compile_options(${target} PRIVATE ${OPTIONS})
    target_compile_definitions(${target} PRIVATE NAMEOF_COMPILE_FAIL_CASE=${case_id})
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

    add_test(
        NAME ${target}
        COMMAND ${CMAKE_COMMAND}
            "-DBUILD_DIR=${CMAKE_BINARY_DIR}"
            "-DTARGET_NAME=${target}"
            "-DCONFIG=$<CONFIG>"
            "-DEXPECTED_TEXT=${expected_text}"
            "-DUNEXPECTED_TEXT=${unexpected_text}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/check_compile_fail.cmake"
    )
    set_tests_properties(${target} PROPERTIES RUN_SERIAL TRUE)
endfunction()

make_test(test.cpp test-cpp17 17)
make_test(test_aliases.cpp test_aliases-cpp17 17)
make_test(test_standalone.cpp test-standalone-cpp17 17)

make_compile_fail_test(enum-no-values 1 "nameof::nameof_enum requires at least one reflected enum value" "names_v")
make_compile_fail_test(flags-no-single-bit 2 "nameof::nameof_enum_flag requires at least one reflected single-bit enumerator")
make_compile_fail_test(expression-no-name 3 "Expression does not have a name")
make_compile_fail_test(inverted-range 4 "nameof::customize::enum_range must contain at least one value" "names_v")
make_compile_fail_test(range-too-large 5 "nameof::customize::enum_range must contain fewer than UINT16_MAX values" "names_v")

if("cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    make_test(test.cpp test-cpp20 20)
    make_test(test_aliases.cpp test_aliases-cpp20 20)
endif()

if("cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    make_test(test.cpp test-cpp23 23)
    make_test(test_aliases.cpp test_aliases-cpp23 23)
endif()

if("cxx_std_26" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    make_test(test.cpp test-cpp26 26)
    make_test(test_aliases.cpp test_aliases-cpp26 26)
endif()
