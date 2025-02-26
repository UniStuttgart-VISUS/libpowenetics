// <copyright file="endian.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <CppUnitTest.h>

#include "endian.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace types {

    /// <summary>
    /// Test the utility functions for the endian enumeration.
    /// </summary>
    TEST_CLASS(endian) {

        TEST_METHOD(system) {
            const auto actual = powenetics_endian::system;
            const auto expected = powenetics_endian::little;
            Assert::AreEqual(int(expected), int(actual), L"System endian current (for all intents and purposes, we assume test to run on x86)", LINE_INFO());
        }

    };

} /* namespace functions */
