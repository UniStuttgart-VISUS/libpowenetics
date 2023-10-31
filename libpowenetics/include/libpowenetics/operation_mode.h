// <copyright file="operation_mode.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "libpowenetics/types.h"


/// <summary>
/// Possible modes for <see cref="powenetics_calibrate" />.
/// </summary>
enum LIBPOWENETICS_ENUM powenetics_operation_mode {

    /// <summary>
    /// 
    /// </summary>
    LIBPOWENETICS_ENUM_SCOPE(powenetics_operation_mode, bootload),

    /// <summary>
    /// Starts streaming data from the device.
    /// </summary>
    LIBPOWENETICS_ENUM_SCOPE(powenetics_operation_mode, stream)
};
