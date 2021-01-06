cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch version from header
set(VERSION_REGEX "#define CASS_VERSION[ \t]+\"(.+)\"")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/cass/cass.h"
  VERSION_STRING REGEX ${VERSION_REGEX})
string(REGEX REPLACE ${VERSION_REGEX} "\\1" VERSION_STRING "${VERSION_STRING}")

project(cass LANGUAGES C VERSION ${VERSION_STRING})
set(PROJECT_DESCRIPTION "Assertion library for the C language.")

if(${CMAKE_C_COMPILER_ID} MATCHES "MSVC")
    set(WARNING_FLAGS /W3)
else()
    set(WARNING_FLAGS -Wall -Wextra -pedantic -Wstrict-prototypes -Wshadow
        -Wconversion -Wmissing-prototypes -Wno-unused-parameter -Wsign-conversion
        -Wno-unused-function # TODO: added only to please clangd for now
        -Wno-empty-translation-unit # TODO: added only to please clangd for now
    )
endif()

add_library(cass INTERFACE)
add_library(CASS::cass ALIAS cass)
target_include_directories(cass
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(cass INTERFACE $<$<BOOL:${UNIX}>:m>)

install(TARGETS cass EXPORT cass-targets)
install(DIRECTORY include/ DESTINATION include)

# Export the targets to a script
install(EXPORT cass-targets
    FILE cass-targets.cmake
    NAMESPACE CASS::
    DESTINATION lib/cmake/cass
)
include(CMakePackageConfigHelpers)
configure_package_config_file(cass-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cass-config.cmake
    INSTALL_DESTINATION lib/cmake/cass
    NO_SET_AND_CHECK_MACRO
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cass-config.cmake
    DESTINATION lib/cmake/cass
)
