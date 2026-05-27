// <copyright file="endian.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_LIBPOWENETICS_ENDIAN_H)
#define _LIBPOWENETICS_ENDIAN_H
#pragma once

#include <cinttypes>

#include "libpowenetics/api.h"


/// <summary>
/// Defines the two possible endians we support anlong with a constant
/// representing the native endian of the system
/// </summary>
enum class powenetics_endian : std::uint32_t {

    /// <summary>
    /// Little endian representation with least significant byte coming
    /// first in memory.
    /// </summary>
    little = 0x04,

    /// <summary>
    /// Big endian representation with most significant byte coming first in
    /// memory.
    /// </summary>
    big = 0x01,

    /// <summary>
    /// Endian of the system the code is running on.
    /// </summary>
    system = 0x01020304 & 0xFF
};

#endif /* !defined(_LIBPOWENETICS_ENDIAN_H) */
