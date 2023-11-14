// <copyright file="serial.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <CppUnitTest.h>

#include <algorithm>

#include "convert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace functions {

    /// <summary>
    /// Test the data conversion functions that interpret the byte stream from
    /// the device.
    /// </summary>
    TEST_CLASS(convert) {

        TEST_METHOD(to_uint16) {
            {
                std::uint8_t input[] = { 0, 0 };
                Assert::AreEqual(int(0), int(::to_uint16(input)), L"0, 0", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 0, 1 };
                Assert::AreEqual(int(1), int(::to_uint16(input)), L"0, 1", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 1, 0 };
                Assert::AreEqual(int(256), int(::to_uint16(input)), L"1, 0", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 0x78, 0x5F };
                Assert::AreEqual(int(30815), int(::to_uint16(input)), L"0x78, 0x5F", LINE_INFO());
            }
        }

        TEST_METHOD(to_uint24) {
            {
                std::uint8_t input[] = { 0, 0, 0 };
                Assert::AreEqual(int(0), int(::to_uint24(input)), L"0, 0, 0", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 0, 0, 1 };
                Assert::AreEqual(int(1), int(::to_uint24(input)), L"0, 0, 1", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 0, 1, 0 };
                Assert::AreEqual(int(256), int(::to_uint24(input)), L"0, 1, 0", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 0, 1, 0 };
                Assert::AreEqual(int(256), int(::to_uint24(input)), L"0, 1, 0", LINE_INFO());
            }

            {
                std::uint8_t input[] = { 0x29, 0x61, 0x33 };
                Assert::AreEqual(int(2711859), int(::to_uint24(input)), L"0x29, 0x61, 0x33", LINE_INFO());
            }
        }

        TEST_METHOD(from_uint16) {
            {
                std::uint8_t buffer[2];
                const std::uint16_t expected = 1;
                ::from_uint16(buffer, expected);
                Assert::AreEqual(int(expected), int(::to_uint16(buffer)), L"1", LINE_INFO());
            }

            {
                std::uint8_t buffer[2];
                const std::uint16_t expected = 256;
                ::from_uint16(buffer, expected);
                Assert::AreEqual(int(expected), int(::to_uint16(buffer)), L"256", LINE_INFO());
            }

            {
                std::uint8_t buffer[2];
                const std::uint16_t expected = 17000;
                ::from_uint16(buffer, expected);
                Assert::AreEqual(int(expected), int(::to_uint16(buffer)), L"17000", LINE_INFO());
            }
        }

        TEST_METHOD(from_uint24) {
            {
                std::uint8_t buffer[3];
                const std::uint32_t expected = 1;
                ::from_uint24(buffer, expected);
                Assert::AreEqual(expected, ::to_uint24(buffer), L"1", LINE_INFO());
            }

            {
                std::uint8_t buffer[3];
                const std::uint32_t expected = 256;
                ::from_uint24(buffer, expected);
                Assert::AreEqual(expected, ::to_uint24(buffer), L"256", LINE_INFO());
            }

            {
                std::uint8_t buffer[3];
                const std::uint32_t expected = 17000;
                ::from_uint24(buffer, expected);
                Assert::AreEqual(expected, ::to_uint24(buffer), L"17000", LINE_INFO());
            }

            {
                std::uint8_t buffer[3];
                const std::uint32_t expected = 70000;
                ::from_uint24(buffer, expected);
                Assert::AreEqual(expected, ::to_uint24(buffer), L"70000", LINE_INFO());
            }
        }
    };

} /* namespace functions */
