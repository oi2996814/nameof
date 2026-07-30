cmake_minimum_required(VERSION 3.22)

if(DEFINED GEN_PKG_CONFIG_TEST_CASE)
    project(gen_pkg_config_test
        VERSION 1.2.3
        DESCRIPTION "GenPkgConfig test package"
        HOMEPAGE_URL "https://github.com/Neargye/nameof"
        LANGUAGES CXX
    )

    if(NOT NAMEOF_SOURCE_DIR)
        message(FATAL_ERROR "NAMEOF_SOURCE_DIR is required")
    endif()

    include(GNUInstallDirs)
    include("${NAMEOF_SOURCE_DIR}/cmake/GenPkgConfig/GenPkgConfig.cmake")

    if(GEN_PKG_CONFIG_TEST_CASE STREQUAL "interface")
        add_library(interface-probe INTERFACE)
        target_include_directories(interface-probe
            INTERFACE "$<INSTALL_INTERFACE:include>"
        )
        configure_pkg_config_file(
            interface-probe
            COMPONENT pkgconfig
            DESCRIPTION "GenPkgConfig interface probe"
            REQUIRES "public-dependency >= 1.0"
            CONFLICTS "old-dependency < 1.0"
        )
    elseif(GEN_PKG_CONFIG_TEST_CASE STREQUAL "object")
        add_library(gen_pkg_config_test OBJECT gen_pkg_config.cpp)
        target_compile_definitions(gen_pkg_config_test
            PRIVATE GEN_PKG_CONFIG_LIBRARY
        )
        configure_pkg_config_file(
            gen_pkg_config_test
            NAME object-probe
            DESCRIPTION "Object probe keeps @UNDEFINED@ metadata"
        )
    elseif(GEN_PKG_CONFIG_TEST_CASE STREQUAL "static")
        add_library(gen_pkg_config_test STATIC gen_pkg_config.cpp)
        target_compile_definitions(gen_pkg_config_test
            PRIVATE GEN_PKG_CONFIG_LIBRARY
        )
        install(TARGETS gen_pkg_config_test
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
        configure_pkg_config_file(
            gen_pkg_config_test
            NAME static-probe
        )
    else()
        message(FATAL_ERROR
            "Unknown GEN_PKG_CONFIG_TEST_CASE: ${GEN_PKG_CONFIG_TEST_CASE}"
        )
    endif()
else()
    project(nameof_consumer LANGUAGES CXX)

    enable_testing()

    find_package(nameof CONFIG REQUIRED)

    add_executable(nameof_consumer main.cpp)
    target_link_libraries(nameof_consumer PRIVATE nameof::nameof)
    target_compile_features(nameof_consumer PRIVATE cxx_std_17)
    set_target_properties(nameof_consumer PROPERTIES CXX_EXTENSIONS OFF)

    add_test(NAME nameof_consumer COMMAND nameof_consumer)
endif()
