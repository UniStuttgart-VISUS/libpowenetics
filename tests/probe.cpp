// <copyright file="probe.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <CppUnitTest.h>

#include "probe.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace classes {

    /// <summary>
    /// Test the <see cref="powenetics_probe" /> helper class that allows us to
    /// discover devices attached to the machine we are running on.
    /// </summary>
    TEST_CLASS(probe) {

        TEST_METHOD(candidates) {
            auto candidates = powenetics_probe::candidates();
        }

    };

} /* namespace functions */
