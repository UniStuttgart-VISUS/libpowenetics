﻿// <copyright file="serial.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <CppUnitTest.h>

#include <algorithm>

#include "libpowenetics/serial.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace functions {

    /// <summary>
    /// Test the utility functions for serial ports.
    /// </summary>
    TEST_CLASS(serial) {

        TEST_METHOD(create_default_config) {
            powenetics_serial_configuration config;
            ::ZeroMemory(&config, sizeof(config));

            {
                auto actual = ::powenetics_initialise_serial_configuration(&config);
                Assert::AreEqual(E_INVALIDARG, actual, L"Invalid argument if version not set", LINE_INFO());
            }

            {
                config.version = 2;
                auto actual = ::powenetics_initialise_serial_configuration(&config);
                Assert::AreEqual(S_OK, actual, L"Initialisation succeeded", LINE_INFO());
                Assert::AreEqual(std::uint32_t(921600), config.baud_rate, L"baud_rate set", LINE_INFO());
                Assert::AreEqual(std::uint8_t(8), config.data_bits, L"data_bits set", LINE_INFO());
                Assert::AreEqual(int(powenetics_parity::none), int(config.parity), L"data_bits set", LINE_INFO());
                Assert::AreEqual(int(powenetics_stop_bits::one), int(config.stop_bits), L"stop_bits set", LINE_INFO());
            }
        }

    };

} /* namespace functions */
