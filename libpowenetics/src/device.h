// <copyright file="device.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <array>
#include <atomic>
#include <cinttypes>
#include <string>
#include <thread>
#include <vector>

#include "libpowenetics/api.h"
#include "libpowenetics/sample.h"
#include "libpowenetics/serial.h"

#include "stream_parser_v2.h"
#include "stream_state.h"


/// <summary>
/// This is the abstraction of the power measurement device which holds the
/// handle of the serial port from which we receive our samples.
/// </summary>
struct LIBPOWENETICS_TEST_API powenetics_device final {

public:

    /// <summary>
    /// The type used to pass around devices from probing.
    /// </summary>
    typedef std::unique_ptr<powenetics_device> device_type;

    /// <summary>
    /// The type used to represent a single byte.
    /// </summary>
    typedef stream_parser_v2::byte_type byte_type;

    /// <summary>
    /// The type of a string.
    /// </summary>
    typedef std::basic_string<powenetics_char> string_type;

    /// <summary>
    /// Gets the paths of all COM ports on the system that might be used to
    /// connect a Powenetics device.
    /// </summary>
    /// <returns>The paths of all COM ports on the system.</returns>
    static std::vector<string_type> probe_candidates(void);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    powenetics_device(void) noexcept;

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~powenetics_device(void) noexcept;

    /// <summary>
    /// Calibrates the given channel to the given current.
    /// </summary>
    /// <param name="channel"></param>
    /// <param name="quantity"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    HRESULT calibrate(_In_ const std::uint8_t channel,
        _In_ const powenetics_quantity quantity,
        _In_ const std::uint32_t value) noexcept;

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
    /// Reads at mode <paramref name="cnt" /> bytes from the serial port.
    /// </summary>
    /// <remarks>
    /// This method must not be called while the device is streaming. Only
    /// the streaming thread within the object may read at this point.
    /// </remarks>
    /// <param name="dst">A buffer that is able to receive at least
    /// <paramref name="cnt" /> bytes.</param>
    /// <param name="cnt">The size of <paramref name="dst" /> on entry, the
    /// number of bytes written on successful exit.</param>
    /// <returns></returns>
    HRESULT read(_Out_writes_(cnt) byte_type *dst,
        _Inout_ std::size_t &cnt) noexcept;

    /// <summary>
    /// Instruct the device to clear all calibration.
    /// </summary>
    HRESULT reset_calibration(void) noexcept;

    /// <summary>
    /// Start streaming data from the device and deliver it to the given
    /// <paramref name="callback" /> function.
    /// </summary>
    HRESULT start(_In_ const powenetics_data_callback callback,
        _In_opt_ void *context) noexcept;

    /// <summary>
    /// Asks the streaming thread to stop and waits for it exit.
    /// </summary>
    HRESULT stop(void) noexcept;

    /// <summary>
    /// Synchronously write the given data to the serial port.
    /// </summary>
    /// <remarks>
    /// The method makes best effort to write all <paramref name="cnt" />
    /// bytes. If this is not possible, it will fail with an error code
    /// indicating the reason.
    /// </remarks>
    /// <param name="data">A pointer to at least <paramref name="cnt" />
    /// bytes of valid data.</param>
    /// <param name="cnt">The number of bytes to write.</param>
    /// <returns></returns>
    HRESULT write(_In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt) noexcept;

    /// <summary>
    /// Synchronously write the given data to the serial port.
    /// </summary>
    template<std::size_t N>
    inline HRESULT write(_In_ const std::array<byte_type, N>& data) noexcept {
        return this->write(data.data(), data.size());
    }

    /// <summary>
    /// Synchronously write the given data to the serial port.
    /// </summary>
    inline HRESULT write(_In_ const std::vector<byte_type>& data) noexcept {
        return this->write(data.data(), data.size());
    }

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
    /// Check whether the thread is still in
    ///  <see cref="stream_state::running" />.
    /// </summary>
    inline bool check_running(void) noexcept {
        auto state = this->_state.load(std::memory_order::memory_order_acquire);
        return (state == stream_state::running);
    }

    /// <summary>
    /// Checks that the streaming thread is stopped.
    /// </summary>
    HRESULT check_stopped(void) noexcept;

    /// <summary>
    /// Checks whether <see cref="_handle" /> is valid.
    /// </summary>
    HRESULT check_valid(void) noexcept;

    /// <summary>
    /// The method executed in <see cref="_thread" /> to continuously read data
    /// from the serial port.
    /// </summary>
    /// <remarks>
    /// This thread continues reading data from <see cref="_handle" /> and
    /// processes these until one of the following conditions is met: the
    /// I/O failes due to <see cref="_handle" /> being closer, or the
    /// <see cref="_state" /> is set to <see cref="stream_state::stopping" />.
    /// </remarks>
    void do_read(void);

    powenetics_data_callback _callback;
    void *_context;
    handle_type _handle;
    std::atomic<stream_state> _state;
    std::thread _thread;
};
