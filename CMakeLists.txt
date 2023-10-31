# CMakeLists.txt
# Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
# Licensed under the MIT licence. See LICENCE file in the project root for detailed information.

cmake_minimum_required(VERSION 3.18.0)
project(powenetics)


# User-configurable options
option(POWENETICS_BuildTests "Build the test driver" OFF)


#  Global compiler options, which are derived from the settings above.
add_compile_definitions(UNICODE _UNICODE)
if (POWENETICS_BuildTests)
    add_compile_definitions(LIBPOWENETICS_EXPOSE_TO_TESTING)
endif ()


# Build the library itself.
add_subdirectory(libpowenetics)


# Build the test driver if possible. The check for that is a first-class hack
# adapted from https://jslav.livejournal.com/13059.html to get  access to the VC
# installation directory.
string(TOLOWER ${CMAKE_MAKE_PROGRAM} MakeProgramme)
if ("${MakeProgramme}" MATCHES "/msbuild.+")
    string(REGEX REPLACE "/msbuild.+" "/VC" VcInstallDir ${MakeProgramme})
    set(PoweneticsCanBuildTests TRUE)
else ()
    set(PoweneticsCanBuildTests FALSE)
endif ()

if (PoweneticsCanBuildTests AND POWENETICS_BuildTests)
    add_subdirectory(tests)
endif ()


# Build the test for the C API.
add_subdirectory(cclient)
