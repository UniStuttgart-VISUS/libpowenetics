// <copyright file="endian.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

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
    little = 0x03020100,

    /// <summary>
    /// Big endian representation with most significant byte coming first in
    /// memory.
    /// </summary>
    big = 0x00010203,

    /// <summary>
    /// Endian of the system the code is running on.
    /// </summary>
    system = '\0\1\2\3'
};
