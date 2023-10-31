﻿// <copyright file="timestamp.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <CppUnitTest.h>

#include <algorithm>

#include <libpowenetics/timestamp.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace functions {

    TEST_CLASS(timestamp) {

        TEST_METHOD(create_from_stl) {
            const auto actual = static_cast<std::int64_t>(::_powenetics_make_timestamp());
            const auto expected = static_cast<std::int64_t>(::powenetics_make_timestamp());
            const auto dt = std::abs(expected - actual);
            Assert::IsTrue(expected - actual < 100, L"STL timestamp close to FILETIME", LINE_INFO());
        }

    };

} /* namespace functions */