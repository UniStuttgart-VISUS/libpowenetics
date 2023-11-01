// <copyright file="responses.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "libpowenetics/api.h"

#include <array>


namespace responses_v2 {

    /// <summary>
    /// The response from the device indicating that calibration failed.
    /// </summary>
    static constexpr std::array<std::uint8_t, 2> calibration_error {
        0xFE, 0xFE
    };

    /// <summary>
    /// The response from the device indicating the calibration succeeded.
    /// </summary>
    static constexpr std::array<std::uint8_t, 2> calibration_success {
        0xFC, 0xFC
    };

    /// <summary>
    /// The delimiter for segments in the data stream.
    /// </summary>
    static constexpr std::array<std::uint8_t, 2> segment_delimiter {
        0xCA, 0xAC
    };

} /* namespace responses_v2 */
