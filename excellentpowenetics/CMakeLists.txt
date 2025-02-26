# CMakeLists.txt
# Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(excellentpowenetics)

include(FetchContent)

# Windows Implementation Library
FetchContent_Declare(wil
    URL "https://github.com/microsoft/wil/archive/v1.0.230202.1.tar.gz"
)
option(FAST_BUILD "" OFF)
option(WIL_BUILD_PACKAGING "" OFF)
option(WIL_BUILD_TESTS "" OFF)
FetchContent_GetProperties(wil)
if (NOT wil_POPULATED)
    FetchContent_Populate(wil)
    add_subdirectory(${wil_SOURCE_DIR} ${wil_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()
mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_WIL
    FETCHCONTENT_UPDATES_DISCONNECTED_WIL
    FAST_BUILD
    WIL_BUILD_PACKAGING
    WIL_BUILD_TESTS)


# Collect source files.
file(GLOB_RECURSE HeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" "*.inl")
file(GLOB_RECURSE SourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")


# Define the output.
add_executable(${PROJECT_NAME} ${HeaderFiles} ${SourceFiles})


# Configure the linker
target_link_libraries(${PROJECT_NAME} PRIVATE libpowenetics WIL)


# Grab libpowenetics and copy it to the output directory such that we can run it
# for debugging purposes without installing it.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS)
