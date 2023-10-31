// <copyright file="device.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <array>
#include <cinttypes>
#include <thread>
#include <vector>

#include "libpowenetics/api.h"
#include "libpowenetics/operation_mode.h"
#include "libpowenetics/sample.h"
#include "libpowenetics/serial.h"

#include "stream_parser_v2.h"



/// <summary>
/// This is the abstraction of the power measurement device which holds the
/// handle of the serial port from which we receive our samples.
/// </summary>
struct LIBPOWENETICS_TEST_API powenetics_device final {

public:

    /// <summary>
    /// The type used to represent a single byte.
    /// </summary>
    typedef stream_parser_v2::byte_type byte_type;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    powenetics_device(void) noexcept;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~powenetics_device(void) noexcept;

    HRESULT calibrate(void) noexcept;

    /// <summary>
    /// Close the serial port connection to the device.
    /// </summary>
    HRESULT close(void) noexcept;

    /// <summary>
    /// Opens and configures the specified COM port if the device has not
    /// yet been opened.
    /// </summary>
    HRESULT open(_In_z_ const powenetics_char *com_port,
        _In_ const powenetics_serial_configuration *config) noexcept;

    /// <summary>
    /// Instruct the device to clear all calibration.
    /// </summary>
    HRESULT reset_calibration(void) noexcept;

    /// <summary>
    /// Start streaming data from the device and deliver it to the given
    /// <paramref name="callback" /> function.
    /// </summary>
    HRESULT start_streaming(_In_ const powenetics_data_callback callback,
        _In_opt_ void *context) noexcept;

private:

#if defined(_WIN32)
    typedef HANDLE handle_type;
#else /* defined(_WIN32) */
    typedef int handle_type;
#endif /* defined(_WIN32) */

#if defined(_WIN32)
    static constexpr handle_type invalid_handle = INVALID_HANDLE_VALUE;
#else /* defined(_WIN32) */
    static constexpr handle_type invalid_handle = -1;
#endif /* defined(_WIN32) */

    /// <summary>
    /// The method executed in <see cref="_thread" /> to continuously read data
    /// from the serial port.
    /// </summary>
    void read(void);

    /// <summary>
    /// Synchronously write the given data to the serial port.
    /// </summary>
    HRESULT write(_In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt) noexcept;

    /// <summary>
    /// Synchronously write the given data to the serial port.
    /// </summary>
    template<std::size_t N> inline HRESULT write(
            _In_ const std::array<byte_type, N>& data) noexcept {
        return this->write(data.data(), data.size());
    }

    /// <summary>
    /// Synchronously write the given data to the serial port.
    /// </summary>
    inline HRESULT write(_In_ const std::vector<byte_type>& data) noexcept {
        return this->write(data.data(), data.size());
    }

    powenetics_data_callback _callback;
    void *_context;
    handle_type _handle;
    std::thread _thread;
};
