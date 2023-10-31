# CMakeLists.txt
# Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
# Licensed under the MIT licence. See LICENCE file in the project root for detailed information.

cmake_minimum_required(VERSION 3.24.0)
project(powenetics)


# Build the library itself.
add_subdirectory(libpowenetics)

# Build the test driver if possible. The check for that is a first-class hack
# adapted from https://jslav.livejournal.com/13059.html to get  access to the VC
# installation directory.
string(TOLOWER ${CMAKE_MAKE_PROGRAM} MakeProgramme)
if ("${MakeProgramme}" MATCHES "/msbuild.+")
    string(REGEX REPLACE "/msbuild.+" "/VC" VcInstallDir ${MakeProgramme})
    set(PoweneticsBuildTests TRUE)
else ()
    set(PoweneticsBuildTests FALSE)
endif ()

if (PoweneticsBuildTests)
    add_subdirectory(tests)
endif ()
