﻿// <copyright file="powenetics.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "libpowenetics/api.h"
#include "libpowenetics/operation_mode.h"
#include "libpowenetics/serial.h"


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/// <summary>
/// Performs the calibration of the device or erases an existing one.
/// </summary>
/// <param name="handle"></param>
/// <param name="mode"></param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_HANDLE</c> if <paramref name="handle" /> is invalid,
/// <c>WS_E_INVALID_OPERATION</c> if the object provided is not in an
/// acceptable state to perform the operation, namely if the serial
/// connection is not open,
/// any platform-specific I/O error if writing to or reading from the
/// device failed.</returns>
HRESULT LIBPOWENETICS_API powenetics_calibrate(
    _In_ const powenetics_handle handle);

/// <summary>
/// Closes the handle to the given Powenetics v2 power measurement device.
/// </summary>
/// <param name="handle"></param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_HANDLE</c> if <paramref name="handle" /> is invalid.</returns>
HRESULT LIBPOWENETICS_API powenetics_close(_In_ const powenetics_handle handle);

/// <summary>
/// Opens a handle to the Powenetics v2 power measurement device connected to
/// the specified serial port.
/// </summary>
/// <param name="out_handle"></param>
/// <param name="com_port"></param>
/// <param name="config"></param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_INVALIDARG</c> if either <paramref name="com_port "/> or
/// <paramref name="config" /> is <c>nullptr</c>,
/// <c>WS_E_INVALID_OPERATION</c> if the device has already been opened
/// before, a platform-specific error code if accessing the selected
/// serial port failed.</returns>
HRESULT LIBPOWENETICS_API powenetics_open(_Out_ powenetics_handle *out_handle,
    _In_z_ const powenetics_char *com_port,
    _In_ const powenetics_serial_configuration *config);

/// <summary>
/// Resets the calibration of the device identified by the given handle.
/// </summary>
/// <param name="handle">The handle for a Powenetics v2 device.</param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_HANDLE</c> if <paramref name="handle" /> is invalid,
/// <c>WS_E_INVALID_OPERATION</c> if the device is not in a valid state to
/// perform the operation,
/// a platform-dependent error code if the underlying I/O failed.</returns>
/// </returns>
HRESULT LIBPOWENETICS_API powenetics_reset_calibration(
    _In_ const powenetics_handle handle);

/// <summary>
/// Puts the given Powenetics v2 power measurment device in streaming mode.
/// </summary>
/// <param name="handle">The handle for a Powenetics v2 device.</param>
/// <returns></returns>
HRESULT LIBPOWENETICS_API powenetics_start_streaming(
    _In_ const powenetics_handle handle);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */