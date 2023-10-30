// <copyright file="commands.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "libpowenetics/api.h"

#include <array>


namespace commands_v2 {

    /// <summary>
    /// TODO:
    /// </summary>
    constexpr std::array<std::uint8_t, 9> bootload_mode {
        0xCA, 0xAC, 0xBD, 0x55, 0x50, 0x44, 0x41, 0x54, 0x45
    };

    /// <summary>
    /// Indicates to the device that the calibration is valid.
    /// </summary>
    constexpr std::array<std::uint8_t, 4> calibration_ok {
        0xCA, 0xAC, 0xBD, 0x01
    };

    /// <summary>
    /// Requests the device to clear its current calibration.
    /// </summary>
    constexpr std::array<std::uint8_t, 4> clear_calibration {
        0xCA, 0xAC, 0xBD, 0x00
    };

    /// <summary>
    /// Instructs the device to start streaming data.
    /// </summary>
    constexpr std::array<std::uint8_t, 4> stream_mode {
         0xCA, 0xAC, 0xBD, 0x90
    };

} /* namespace commands_v2 */


    /*
        public static class StatusCodes
    {
        public const string Calibration_Error = "FE-FE";
        public const string Calibration_OK = "FC-FC";
    }
    */
