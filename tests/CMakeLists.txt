# CMakeLists.txt
# Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
# Licensed under the MIT licence. See LICENCE file in the project root for detailed information.

project(tests)


if (NOT VcInstallDir)
    message(FATAL_ERROR "An MSBuild-based process is required for building the tests.")
endif ()


# Collect source files.
file(GLOB_RECURSE HeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" "*.inl")
file(GLOB_RECURSE SourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")


# Define the output.
add_library(${PROJECT_NAME} SHARED ${HeaderFiles} ${SourceFiles})


# Configure the compiler.
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)


# In the test driver, the compiler needs to know about the private includes of
# the library, so we add these manually.
target_include_directories(${PROJECT_NAME} PRIVATE ${LibpoweneticsTestInclude})


# Configure the linker: besides the library to test, we also need to link the
# Visual Studio testing framework.
target_link_directories(${PROJECT_NAME} PRIVATE "${VcInstallDir}/Auxiliary/VS/UnitTest/lib/$(LibrariesArchitecture)")
target_link_libraries(${PROJECT_NAME} PRIVATE
    Microsoft.VisualStudio.TestTools.CppUnitTestFramework.lib
    libpowenetics)


# Grab the DLLs to be tested and copy them to the output directory such that
# the Visual Studio test driver finds everything.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS)
