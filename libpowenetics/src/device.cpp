// <copyright file="device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "device.h"

#include <cassert>
#include <string>

#if !defined(_WIN32)
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif /* !defined(_WIN32) */

#include "commands.h"
#include "debug.h"
#include "stream_parser_v2.h"
#include "thread_name.h"


/*
 * powenetics_device::powenetics_device
 */
powenetics_device::powenetics_device(void) noexcept
    : _callback(nullptr),
    _context(nullptr),
    _handle(invalid_handle),
    _state(stream_state::stopped) { }


/*
 * powenetics_device::~powenetics_device
 */
powenetics_device::~powenetics_device(void) noexcept {
    this->close();
    // Note: closing the handle will cause the thread to exit with an I/O error,
    // so we do not need to set the state here (it cannot be used anyway,
    // because we are about to destroy the variable).

    if (this->_thread.joinable()) {
        _powenetics_debug("Waiting for serial reader thread to exit ...\r\n");
        this->_thread.join();
    }
}


/*
 * powenetics_device::calibrate
 */
HRESULT powenetics_device::calibrate(void) noexcept {
    auto retval = this->check_valid();

    // We should not calibrate while we are sampling.
    if (SUCCEEDED(retval)) {
        retval = this->check_stopped();
    }

    //if (mode == 0) //Amps
    //{
    //    int calibration_channel = (int)m.Calibration_Channel.Value - 1;
    //    int calibration_amperes = (int)m.Calibration_Amperes.Value * 1000; //to convert it to mA
    //    int calibration_part1, calibration_part2, calibration_part3;
    //    int Remainder1, Remainder2;
    //    calibration_part1 = calibration_part2 = calibration_part3 = Remainder1 = Remainder2 = 0;


    //    if (calibration_amperes >= 65536)
    //    {
    //        calibration_part1 = calibration_amperes / 65536;
    //        Remainder1 = calibration_amperes % 65536;

    //        calibration_part2 = Remainder1 / 256;
    //        Remainder2 = Remainder1 % 256;
    //        calibration_part3 = Remainder2;
    //    } else if (calibration_amperes < 65536)
    //    {
    //        calibration_part1 = 0;

    //        calibration_part2 = calibration_amperes / 256;
    //        Remainder2 = calibration_amperes % 256;
    //        calibration_part3 = Remainder2;
    //    }


    //    PMD.Write(new byte[]{ 0xCA, (byte)calibration_channel, (byte)calibration_part1, (byte)calibration_part2, (byte)calibration_part3 }, 0, 5);
    //    await Task.Delay(100);

    //    //FE-FE
    //    if (PMD_Response.Contains(StatusCodes.Calibration_Error)) m.PMD_Cal_Response.Text = "Error!";
    //    else if (PMD_Response.Contains(StatusCodes.Calibration_OK)) m.PMD_Cal_Response.Text = "Cal OK!";
    //} else if (mode == 1)
    //{
    //    PMD.Write(PMDCommands.Clear_Calibration, 0, 4);
    //    m.PMD_Cal_Response.Text = "Cal Reset!";
    //}
    return E_NOTIMPL;
}


/*
 * powenetics_device::close
 */
HRESULT powenetics_device::close(void) noexcept {
#if defined(_WIN32)
    auto retval = ::CloseHandle(this->_handle)
        ? S_OK
        : HRESULT_FROM_WIN32(::GetLastError());
#else /* defined(_WIN32) */
    auto retval = (::close(this->_handle) == 0)
        ? S_OK
        : static_cast<HRESULT>(-errno);
#endif /* defined(_WIN32) */

    this->_handle = invalid_handle;
    return retval;
}


/*
 * powenetics_device::open
 */
HRESULT powenetics_device::open(
        _In_z_ const powenetics_char *com_port,
        _In_ const powenetics_serial_configuration *config) noexcept {
    assert(com_port != nullptr);
    assert(config != nullptr);

    if (this->_handle != invalid_handle) {
        _powenetics_debug("Tried opening a powenetics_device that is already "
            "connected...\r\n");
        return E_NOT_VALID_STATE;
    }

#if defined(_WIN32)
    this->_handle = ::CreateFileW(com_port, GENERIC_READ | GENERIC_WRITE, 0,
        nullptr, OPEN_EXISTING, 0, NULL);
    if (this->_handle == invalid_handle) {
        auto retval = HRESULT_FROM_WIN32(::GetLastError());
        _powenetics_debug("CreateFile on COM port failed.\r\n");
        return retval;
    }

    {
        DCB dcb;
        ::ZeroMemory(&dcb, sizeof(dcb));
        dcb.DCBlength = sizeof(dcb);

        if (!::GetCommState(this->_handle, &dcb)) {
            auto retval = HRESULT_FROM_WIN32(::GetLastError());
            _powenetics_debug("Retrieving state of COM port failed.\r\n");
            return retval;
        }

        dcb.BaudRate = static_cast<DWORD>(config->baud_rate);
        dcb.ByteSize = static_cast<BYTE>(config->data_bits);
        dcb.Parity = static_cast<BYTE>(config->parity);
        dcb.StopBits = static_cast<BYTE>(config->stop_bits);

        if (!::SetCommState(this->_handle, &dcb)) {
            auto retval = HRESULT_FROM_WIN32(::GetLastError());
            _powenetics_debug("Updating state of COM port failed.\r\n");
            return retval;
        }
    }

#else /* defined(_WIN32) */
    this->_handle = open(com_port, O_RDWR);
    if (this->_handle == invalid_handle) {
        auto retval = static_cast<HRESULT>(-errno);
        _powenetics_debug("open on COM port failed.\r\n");
        return retval;
    }

    {
        termios tty;

        if (::tcgetattr(this->_handle, &tty) != 0) {
            auto retval = static_cast<HRESULT>(-errno);
            _powenetics_debug("Retrieving state of COM port failed.\r\n");
            return retval;
        }

        // Baud rate
        ::cfsetispeed(&tty, config->baud_rate);
        ::cfsetospeed(&tty, config->baud_rate);

        // Data bits: erase any existing value and set what we got as input.
        tty.c_cflag &= ~CSIZE;
        switch (config->data_bits) {
            case 5:
                tty.c_cflag |= CS5;
                break;

            case 6:
                tty.c_cflag |= CS6;
                break;

            case 7:
                tty.c_cflag |= CS7;
                break;

            case 8:
                tty.c_cflag |= CS8;
                break;

            default:
                _powenetics_debug("An invalid value for the data bits has been "
                    "provided. Only values within [5, 8] are acceptable.\r\n");
                return E_INVALIDARG;
        }

        // Parity:
        switch (config->parity) {
            powenetics_parity::none:
            tty.c_cflag &= ~PARENB;
            break;

            default:
                // TODO: might need adjustment.
                tty.c_cflag |= PARENB;
                break;
        }

        // Stop bits: Clearing the bits means one, setting them means two.
        switch (config->stop_bits) {
            case powenetics_stop_bits::one:
                tty.c_cflag &= ~CSTOPB;
                break;

            case powenetics_stop_bits::two:
                tty.c_cflag |= CSTOPB;
                break;

            default:
                _powenetics_debug("An invalid value was passed for the number "
                    "of stop bits. Only powenetics_stop_bits::one or
                    "powenetics_stop_bits::two are acceptable.\r\n");
                return E_INVALIDARG;
        }

        // Disable "canonical mode": In canonical mode, we only receive data if
        // a new line character was received, which is not what we want for the
        // byte stream from the deivce.
        tty.c_lflag &= ~ICANON;

        // Disable echoing of input.
        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;

        // Disable interpretation of bytes that might be signal characters and
        // any preprocessing of data before they are handed over to us. We want
        //  everything we get to be interpreted as data.
        tty.c_lflag &= ~ISIG;
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR
            | ICRNL);

        // Same for the output: just send what we write.
        tty.c_oflag &= ~(OPOST | ONLCR);


        if (::tcsetattr(this->_handle, TCSANOW, &tty) != 0) {
            auto retval = static_cast<HRESULT>(-errno);
            _powenetics_debug("Updating state of COM port failed.\r\n");
            return retval;
        }
    }
#endif /* defined(_WIN32) */

    return S_OK;
}


/*
 * powenetics_device::reset_calibration
 */
HRESULT powenetics_device::reset_calibration(void) noexcept {
    auto retval = this->check_valid();

    // We should not reset the calibration while we are sampling.
    if (SUCCEEDED(retval)) {
        retval = this->check_stopped();
    }

    if (SUCCEEDED(retval)) {
        retval = this->write(commands_v2::clear_calibration);
    }

    return retval;
}


/*
 * powenetics_device::start
 */
HRESULT powenetics_device::start(
        _In_ const powenetics_data_callback callback,
        _In_opt_ void *context) noexcept {
    auto retval = this->check_valid();

    if (SUCCEEDED(retval)) {
        auto expected = stream_state::stopped;
        auto succeeded = this->_state.compare_exchange_strong(expected,
            stream_state::stopping, std::memory_order::memory_order_acq_rel);
        assert(!this->_thread.joinable() || !succeeded);
        if (!succeeded) {
            _powenetics_debug("The Powenetics device is already streaming "
                "data.\r\n");
            retval = E_NOT_VALID_STATE;
        }
    }

    if (SUCCEEDED(retval)) {
        this->_callback = callback;
        this->_context = context;

        if (this->_callback == nullptr) {
            _powenetics_debug("An invalid data callback has been passed.\n\n");
            retval = E_POINTER;
        }
    }

    if (SUCCEEDED(retval)) {
        retval = this->write(commands_v2::calibration_ok);
    }

    if (SUCCEEDED(retval)) {
        retval = this->write(commands_v2::stream_mode);
    }

    return retval;
}


/*
 * powenetics_device::stop
 */
HRESULT powenetics_device::stop(void) noexcept {
    auto retval = this->check_valid();

    // Note: we do not check 'retval' here, because the sampler thread is not
    // dependent on the handle and we want it to exit under any circumstance.
    {
        auto expected = stream_state::running;
        auto succeeded = this->_state.compare_exchange_strong(expected,
            stream_state::stopping, std::memory_order::memory_order_acq_rel);

        // Return the error only if it does not mask a previous one.
        if (SUCCEEDED(retval) && !succeeded) {
            _powenetics_debug("An attempt to stop streaming data was made on a "
                "device that was not streaming data in the first place.\n\n");
            retval = E_NOT_VALID_STATE;
        }
    }

    // If the handle is valid, put the device back in bootload mode. Note that
    // it is important to do that *after* requesting the thread to exit, because
    // if the thread does not receive any data from the device, the I/O will
    // block and the only way to exit is closing the handle.
    if (SUCCEEDED(retval)) {
        retval = this->write(commands_v2::bootload_mode);
    }

    // Our contract states that the sampler thread must not run anymore once the
    // methods exits, so we wait for the thread to exit.
    if (this->_thread.joinable()) {
        this->_thread.join();
    }

    return retval;
}


/*
 * powenetics_device::check_stopped
 */
HRESULT powenetics_device::check_stopped(void) noexcept {
    const auto s = this->_state.load(std::memory_order::memory_order_acquire);
    const auto succeeded = (s == stream_state::stopped);

    if (!succeeded) {
        _powenetics_debug("The Powenetics v2 sampler thread is either running "
            "or in a transitional state.\r\n");
    }

    return succeeded ? S_OK : E_NOT_VALID_STATE;
}


/*
 * powenetics_device::check_valid
 */
HRESULT powenetics_device::check_valid(void) noexcept {
    const auto succeeded = (this->_handle != invalid_handle);

    if (!succeeded) {
        _powenetics_debug("The connection to the Powenetics v2 device has not "
            "been established.\r\n");
    }

    return succeeded ? S_OK : E_NOT_VALID_STATE;
}


/*
 * powenetics_device::read
 */
void powenetics_device::read(void) {
    set_thread_name("powenetics sampler");

    // Signal to everyone that we are now running. If this fails (with a strong
    // CAS), someone else has manipulated the '_state' variable in the meantime.
    // This is illegal. No one may change the state during startup except for
    // the sampler thread itself at this very point.
    {
        auto expected = stream_state::starting;
        if (!this->_state.compare_exchange_strong(expected,
                stream_state::running,
                std::memory_order::memory_order_acq_rel)) {
            _powenetics_debug("The state of the Powenetics v2 sampler thread "
                "was manipulated during startup.\r\n");
            std::abort();
        }
    }

    std::vector<byte_type> buffer;
    buffer.resize(4 * 1024);
    stream_parser_v2 parser;

#if defined(_WIN32)
    DWORD read;

    while (this->check_running() && ::ReadFile(this->_handle,
            buffer.data(),
            static_cast<DWORD>(buffer.size()),
            &read,
            nullptr)) {
        parser.push_back(buffer.data(), read,
                [this](const powenetics_sample &sample) {
            if (this->_callback != nullptr) {
                this->_callback(this, &sample, this->_context);
            }
        });
    }

#else /* defined(_WIN32) */
    while (this->check_running()) {
        const auto read = ::read(this->_handle, buffer.data(), buffer.size());
        if (read > 0) {
            parser.push_back(buffer.data(), read,
                [this](const powenetics_sample &sample) {
                if (this->_callback != nullptr) {
                    this->_callback(this, &sample, this->_context);
                }
            });
        } else if (errno != 0) {
            break;
        }
    }
#endif /* defined(_WIN32) */

    // Indicate that we are done. We do not CAS this from
    // stream_state::stopping, because a request for orderly shutdown is only
    // one way we can get here, the file handle being closed and the I/O failing
    // being the other one. In the latter case, the state will still be
    // stream_state::running at this point.
    this->_state.store(stream_state::stopped,
        std::memory_order::memory_order_release);
}


/*
 * powenetics_device::write
 */
HRESULT powenetics_device::write(_In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt) noexcept {
    assert(this->_handle != invalid_handle);

#if defined(_WIN32)
    auto cur = data;
    auto rem = static_cast<DWORD>(cnt);
    DWORD written = 0;

    while (::WriteFile(this->_handle, cur, rem, &written, nullptr)) {
        if (written == 0) {
            return S_OK;
        }

        assert(written <= rem);
        cur += written;
        rem -= written;
    }

    auto retval = HRESULT_FROM_WIN32(::GetLastError());
    _powenetics_debug("I/O error while sending a command to Powenetics "
        "v2 device.\r\n");
    return retval;

#else /* defined(_WIN32) */
    auto cur = data;
    auto rem = cnt;

    while (rem > 0) {
        auto written = ::write(this->_handle, cur, sizeof(rem));

        if (written < 0) {
            auto retval = static_cast<HRESULT>(-errno);
            _powenetics_debug("I/O error while sending a command to Powenetics "
                "v2 device.\r\n");
            return retval;
        }

        assert(written <= rem);
        cur += written;
        rem -= written;
    }

    return S_OK;
#endif /* defined(_WIN32) */
}
