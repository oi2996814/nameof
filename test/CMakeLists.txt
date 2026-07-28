include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(OPTIONS /W4 /WX)
    check_cxx_compiler_flag(/permissive- HAS_PERMISSIVE_FLAG)
    if(HAS_PERMISSIVE_FLAG)
        set(OPTIONS ${OPTIONS} /permissive-)
    endif()

    check_cxx_compiler_flag(/std:c++20 HAS_CPP20_FLAG)
    check_cxx_compiler_flag(/std:c++23 HAS_CPP23_FLAG)
    check_cxx_compiler_flag(/std:c++latest HAS_CPPLATEST_FLAG)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(OPTIONS -Wall -Wextra -pedantic-errors -Werror)

    check_cxx_compiler_flag(-std=c++20 HAS_CPP20_FLAG)
    check_cxx_compiler_flag(-std=c++23 HAS_CPP23_FLAG)
endif()

function(make_test src target std)
    add_executable(${target} ${src})
    target_compile_options(${target} PRIVATE ${OPTIONS})
    target_include_directories(${target} PRIVATE 3rdparty)
    target_compile_definitions(${target} PRIVATE DOCTEST_CONFIG_USE_STD_HEADERS FMT_HEADER_ONLY)
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    if(std)
        if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            target_compile_options(${target} PRIVATE /std:${std})
        else()
            target_compile_options(${target} PRIVATE -std=${std})
        endif()
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

make_test(test.cpp test-cpp17 c++17)
make_test(test_aliases.cpp test_aliases-cpp17 c++17)

make_compile_fail_test(enum-no-values 1 "nameof::nameof_enum requires at least one reflected enum value" "names_v")
make_compile_fail_test(flags-no-single-bit 2 "nameof::nameof_enum_flag requires at least one reflected single-bit enumerator")
make_compile_fail_test(expression-no-name 3 "Expression does not have a name")
make_compile_fail_test(inverted-range 4 "nameof::customize::enum_range must contain at least one value" "names_v")
make_compile_fail_test(range-too-large 5 "nameof::customize::enum_range must contain fewer than UINT16_MAX values" "names_v")

if(HAS_CPP20_FLAG)
    make_test(test.cpp test-cpp20 c++20)
    make_test(test_aliases.cpp test_aliases-cpp20 c++20)
endif()

if(HAS_CPP23_FLAG)
    make_test(test.cpp test-cpp23 c++23)
    make_test(test_aliases.cpp test_aliases-cpp23 c++23)
endif()

if(HAS_CPPLATEST_FLAG)
    make_test(test.cpp test-cpplatest c++latest)
    make_test(test_aliases.cpp test_aliases-cpplatest c++latest)
endif()
