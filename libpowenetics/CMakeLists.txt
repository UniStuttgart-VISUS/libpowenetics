# CMakeLists.txt
# Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file for details.

project(libpowenetics)


# Grab all the files the target depends on.
set(IncludeDirectory "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SourceDirectory "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE PublicHeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${IncludeDirectory}" "*.h" "*.inl")
file(GLOB_RECURSE PrivateHeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SourceDirectory}" "*.h" "*.inl")
set (HeaderFiles ${PublicHeaderFiles} ${PrivateHeaderFiles})
file(GLOB_RECURSE SourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SourceDirectory}" "*.cpp")
if (WIN32)
    file(GLOB_RECURSE ResourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "*.rc")
else ()
    set(ResourceFiles "")
endif ()

# Group resource scripts into a new filter.
source_group("Resource Files" FILES ${ResourceFiles})


# Define the target
add_library(${PROJECT_NAME} SHARED ${HeaderFiles} ${SourceFiles} ${ResourceFiles})
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBPOWENETICS_EXPORTS)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${IncludeDirectory}>
    PRIVATE
        $<BUILD_INTERFACE:${SourceDirectory}>)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE SetupAPI)
else ()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif ()

if (POWENETICS_UseUdev)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_UDEV)
    target_link_libraries(${PROJECT_NAME} PRIVATE udev)
endif ()


# Provide the internal API to the unit tests.
set(LibpoweneticsTestInclude "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)


# Install the library
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
