cmake_minimum_required(VERSION 3.12)
project(curp VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_TESTS "Build tests for the curp library" ON)

add_library(curp src/curp.c src/trie.c)
target_include_directories(curp PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# shared library
include(GenerateExportHeader)
set_target_properties(curp PROPERTIES 
    POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
generate_export_header(curp)

install(TARGETS curp EXPORT curpTargets DESTINATION lib)
install(FILES include/curp.h ${PROJECT_BINARY_DIR}/curp_export.h
    DESTINATION include)

# export
include(CMakePackageConfigHelpers)
install(EXPORT curpTargets FILE curpTargets.cmake
    DESTINATION lib/cmake/curp)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/curpConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/curp"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/curpConfigVersion.cmake"
    VERSION "${curp_VERSION_MAJOR}.${curp_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/curpConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/curpConfigVersion.cmake
    DESTINATION lib/cmake/curp)
export(EXPORT curpTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/curpTargets.cmake")

# packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${curp_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${curp_VERSION_MINOR}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

# tests
if (BUILD_TESTS)
    message("-- Configuring curp tests")
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(check
        GIT_REPOSITORY https://github.com/libcheck/check.git
        GIT_TAG 0.15.2)
    FetchContent_Declare(theft
        GIT_REPOSITORY https://github.com/silentbicycle/theft.git
        GIT_TAG v0.4.5)
    FetchContent_MakeAvailable(check)
    FetchContent_MakeAvailable(theft) # sin CMakeLists: solo descarga

    # theft usa un Makefile; se compila aquí a partir de sus fuentes
    file(GLOB THEFT_SOURCES ${theft_SOURCE_DIR}/src/*.c)
    add_custom_command(
        OUTPUT ${theft_BINARY_DIR}/bits_lut.h
        COMMAND awk -f ${theft_SOURCE_DIR}/scripts/mk_bits_lut
                > ${theft_BINARY_DIR}/bits_lut.h)
    add_library(theft STATIC ${THEFT_SOURCES} ${theft_BINARY_DIR}/bits_lut.h)
    target_include_directories(theft
        PUBLIC ${theft_SOURCE_DIR}/inc
        PRIVATE ${theft_BINARY_DIR})
    target_compile_definitions(theft PRIVATE _POSIX_C_SOURCE=199309L)
    set_target_properties(theft PROPERTIES C_STANDARD 99)
    target_link_libraries(theft PRIVATE m)

    add_executable(test_curp
        tests/check_curp.c
        tests/check_curp_examples.c
        tests/check_curp_pbt.c)
    target_link_libraries(test_curp curp Check::check theft)

    add_test(NAME curp COMMAND test_curp)
endif()
