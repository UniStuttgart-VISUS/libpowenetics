// <copyright file="powenetics.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#if defined(__cplusplus)
#include <memory>
#endif /* defined(__cplusplus) */

#include "libpowenetics/api.h"
#include "libpowenetics/sample.h"
#include "libpowenetics/serial.h"


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if 0
/// <summary>
/// Performs the calibration of the device or erases an existing one.
/// </summary>
/// <remarks>
/// This feature seems to be experimental in the original implementation. While
/// we ported this behaviour, we strongly recommend reading
/// https://hwbusters.com/gpu/powenetics-users-manual/5/ before doing anything
/// here as this might make your device unusable!
/// </remarks>
/// <param name="handle">A valid handle for a session.</param>
/// <param name="channel">The zero-based channel to calibrate.</param>
/// <param name="quantity">The quantity to be calibrated. At the moment, this
/// must be <see cref="powenetics_quantity::current" />.</param>
/// <param name="milliamperes">The reference current to calibrate to. The unit
/// depends on the quantity being calibrated, and is milliamperes for current.
/// </param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_HANDLE</c> if <paramref name="handle" /> is invalid,
/// <c>E_NOT_VALID_STATE</c> if the object provided is not in an
/// acceptable state to perform the operation, namely if the serial
/// connection is not open,
/// any platform-specific I/O error if writing to or reading from the
/// device failed.</returns>
HRESULT LIBPOWENETICS_API powenetics_calibrate(
    _In_ const powenetics_handle handle,
    _In_ const uint8_t channel,
    _In_ const powenetics_quantity quantity,
    _In_ const uint32_t value);
#endif

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
/// <param name="out_handle">Receives the handle for the power measurement
/// device in case of success,</param>
/// <param name="com_port">The path to the COM port. The format of this path
/// is platform-specific. For instance, on Windows, this would be something
/// like &quot;\\.\COM3&quot;, whereas on Linux, you would use somethin
/// like &quot;/dev/ttyACM0&quot;.</param>
/// <param name="config">The configuration used for the serial port. It is safe
/// to pass <c>nullptr</c>, in which case the function will obtain the default
/// configuration by calling
/// <see cref="powenetics_initialise_serial_configuration" />.</param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_INVALIDARG</c> if either <paramref name="out_handle "/> or
/// <paramref name="com_port" /> is <c>nullptr</c>,
/// <c>E_NOT_VALID_STATE</c> if the device has already been opened
/// before, a platform-specific error code if accessing the selected
/// serial port failed.</returns>
HRESULT LIBPOWENETICS_API powenetics_open(_Out_ powenetics_handle *out_handle,
    _In_z_ const powenetics_char *com_port,
    _In_opt_ const powenetics_serial_configuration *config);

/// <summary>
/// Searches the local machine for Powenetics v2 power measurement
/// devices and returns the paths as multi-sz string.
/// </summary>
/// <remarks>
/// This method may take significant time to check all ports found on the
/// device.
/// </remarks>
/// <param name="out_ports">A buffer to receive at least
/// <paramref name="cnt" /> characters.</param>
/// <param name="cnt">On entry, the number characters that can be saved to
/// <paramref name="out_handles" />, on exit, the number of handles that have
/// actually been written or would be required to store all paths.</param>
/// <returns><c>S_OK</c> in case the operation succeeded,
/// <c>HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)</c> if there were more
/// devices than could be stored to <paramref name="out_ports" />, 
/// <c>HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)</c> if no device at all
/// was found, another error code if establishing the connection to the
/// device failed.</returns>
HRESULT LIBPOWENETICS_API powenetics_probe(
    _Out_writes_opt_z_(*cnt) powenetics_char *out_ports,
    _Inout_ size_t *cnt);

#if 0
/// <summary>
/// Resets the calibration of the device identified by the given handle.
/// </summary>
/// <param name="handle">The handle for a Powenetics v2 device.</param>
/// <returns><c>S_OK</c> in case of success,
/// <c>E_HANDLE</c> if <paramref name="handle" /> is invalid,
/// <c>E_NOT_VALID_STATE</c> if the device is not in a valid state to
/// perform the operation,
/// a platform-dependent error code if the underlying I/O failed.</returns>
/// </returns>
HRESULT LIBPOWENETICS_API powenetics_reset_calibration(
    _In_ const powenetics_handle handle);
#endif

/// <summary>
/// Puts the given Powenetics v2 power measurement device in streaming mode.
/// </summary>
/// <param name="handle">The handle for a Powenetics v2 device.</param>
/// <param name="callback">The callback to be invoked if the device sent a new
/// sample.</param>
/// <param name="context">A user-defined pointer that will be passed to
/// <paramref name="callback" /> along with each sample.</param>
/// <returns></returns>
HRESULT LIBPOWENETICS_API powenetics_start_streaming(
    _In_ const powenetics_handle handle,
    _In_ const powenetics_data_callback callback,
    _In_opt_ void *context);

/// <summary>
/// Returns the given Powenetics n2 power measurement to startup state.
/// </summary>
/// <remarks>
/// This method blocks until the thread delivering the samples actually stopped
/// and it is safe to invalidate any previously installed callback.
/// </remarks>
/// <param name="handle">The handle for a Powenetics v2 device.</param>
/// <returns><c>S_OK</c> in case of success, <c>E_HANDLE</c> if
/// <paramref name="handle" /> is invalid, another error code if for instance
/// I/O with the device failed.</returns>
HRESULT LIBPOWENETICS_API powenetics_stop_streaming(
    _In_ const powenetics_handle handle);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */


#if defined(__cplusplus)
namespace visus {
namespace powenetics {

    /// <summary>
    /// A deleter functor for <see cref="powenetics_handle" />, which can be
    /// used for <see cref="std::unique_ptr" />.
    /// </summary>
    struct handle_deleter final {
        inline void operator ()(powenetics_handle device) const {
            ::powenetics_close(device);
        }
    };

    /// <summary>
    /// A unique pointer to replace <see cref="powenetics_handle" />.
    /// </summary>
    typedef std::unique_ptr<powenetics_device, handle_deleter> unique_handle;

} /* namespace powenetics */
} /* namespace visus */
#endif /* defined(__cplusplus) */
