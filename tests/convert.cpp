// <copyright file="serial.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <CppUnitTest.h>

#include <algorithm>

#include <convert.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace functions {

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
        }

    };

} /* namespace functions */
