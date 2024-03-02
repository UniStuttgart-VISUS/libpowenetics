// <copyright file="commands.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
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
    /// <remarks>
    /// This is the counterpart to <see cref="clear_calibration" /> that will
    /// assign a new calibration to the device. The caller needs to be able to
    /// actually calibrate the device against a precise reference when using
    /// this command.
    /// <remarks>
    constexpr std::array<std::uint8_t, 4> calibration_ok {
        0xCA, 0xAC, 0xBD, 0x01
    };

    /// <summary>
    /// Requests the device to clear its current calibration.
    /// </summary>
    /// <remarks>
    /// This will make the device unusable unless you are able to calibrate it
    /// again. Therefore, this command is not for general use.
    /// </remarks>
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
