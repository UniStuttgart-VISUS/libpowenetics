// <copyright file="responses.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LIBPOWENETICS_RESPONSES_H)
#define _LIBPOWENETICS_RESPONSES_H
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

#endif /* !defined(_LIBPOWENETICS_RESPONSES_H) */
