﻿// <copyright file="serial.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "libpowenetics/api.h"
#include "libpowenetics/types.h"


/// <summary>
/// Possible parities that can be configured in
/// <see cref="powenetics_serial_configuration" />.
/// </summary>
/// <remarks>
/// The numeric values of the enumeration are compatible with the
/// <see cref="DCB" /> structure on Windows.
/// </remarks>
enum LIBPOWENETICS_ENUM powenetics_parity : std::uint8_t {
    none = 0,
    odd = 1,
    even = 2,
    mark = 3,
    space = 4
};


/// <summary>
/// Possible stop bits that can be set in
/// <see cref="powenetics_serial_configuration" />.
/// </summary>
/// <remarks>
/// The numeric values of the enumeration are compatible with the
/// <see cref="DCB" /> structure on Windows.
/// </remarks>
enum LIBPOWENETICS_ENUM powenetics_stop_bits : std::uint8_t {
    one = 0,
    one_point_five = 1,
    two = 2
};


/// <summary>
/// Configures the serial port for talking with a Powenetics v2 device.
/// </summary>
struct LIBPOWENETICS_API powenetics_serial_configuration {

    /// <summary>
    /// The version of the structure.
    /// </summary>
    /// <remarks>
    /// <para>This member allows the library to discern between future versions
    /// of the structure. It must be initialises to 1 in the first version of
    /// the library.</para>
    /// <para>This must be the first member of the struct and any future version
    /// of it.</para>
    /// </remarks>
    std::uint32_t version;

    /// <summary>
    /// The baud rate.
    /// </summary>
    std::uint32_t baud_rate;

    /// <summary>
    /// The number of data bits.
    /// </summary>
    std::uint8_t data_bits;

    /// <summary>
    /// The parity bit.
    /// </summary>
    powenetics_parity parity;

    /// <summary>
    /// The number of stop bits.
    /// </summary>
    powenetics_stop_bits stop_bits;

};


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/// <summary>
/// Applies the default configuration for a Powenetics v2 device to the
/// strucuture passed to the method.
/// </summary>
/// <param name="config">A pointer to the structure to be filled. The version
/// of the structure must have been initialised before the call.</param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_POINTER</c> if <paramref name="config" /> is <c>nullptr</c>,
/// <c>E_INVALIDARG</c> if the version of the configuration has not been
/// initialised or is unsupported by the function.</returns>
HRESULT LIBPOWENETICS_API powenetics_initialise_serial_configuration(
    _In_ powenetics_serial_configuration *config);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */