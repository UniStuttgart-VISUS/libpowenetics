// <copyright file="device.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "device.h"

#include <cassert>

#include "commands.h"
#include "thread_name.h"


/*
 * powenetics_device::powenetics_device
 */
powenetics_device::powenetics_device(void) noexcept
    : _handle(invalid_handle) { }


/*
 * powenetics_device::~powenetics_device
 */
powenetics_device::~powenetics_device(void) noexcept {
    this->close();

    if (this->_thread.joinable()) {
        this->_thread.join();
    }
}


/*
 * powenetics_device::calibrate
 */
HRESULT powenetics_device::calibrate(void) noexcept {
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
    throw "TODO";
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
    if ((com_port == nullptr) || (config == nullptr)) {
        return E_INVALIDARG;
    }

    if (this->_handle != invalid_handle) {
        return WS_E_INVALID_OPERATION;
    }

#if defined(_WIN32)
    this->_handle = ::CreateFileW(com_port, GENERIC_READ | GENERIC_WRITE, 0,
        nullptr, OPEN_EXISTING, 0, NULL);
    if (this->_handle == invalid_handle) {
        return HRESULT_FROM_WIN32(::GetLastError());
    }


    {
        DCB dcb;
        ::ZeroMemory(&dcb, sizeof(dcb));
        dcb.DCBlength = sizeof(dcb);

        if (!::GetCommState(this->_handle, &dcb)) {
            return HRESULT_FROM_WIN32(::GetLastError());
        }

        dcb.BaudRate = static_cast<DWORD>(config->baud_rate);
        dcb.ByteSize = static_cast<BYTE>(config->data_bits);
        dcb.Parity = static_cast<BYTE>(config->parity);
        dcb.StopBits = static_cast<BYTE>(config->stop_bits);

        if (!::SetCommState(this->_handle, &dcb)) {
            return HRESULT_FROM_WIN32(::GetLastError());
        }
    }

#else /* defined(_WIN32) */
    throw "TODO";
#endif /* defined(_WIN32) */

    return S_OK;
}


/*
 * powenetics_device::reset_calibration
 */
HRESULT powenetics_device::reset_calibration(void) noexcept {
    if (this->_handle != invalid_handle) {
        return WS_E_INVALID_OPERATION;
    } else {
        return this->write(commands_v2::clear_calibration);
    }
}


/*
 * powenetics_device::start_streaming
 */
HRESULT powenetics_device::start_streaming(void) noexcept {
    auto retval = S_OK;

    if (SUCCEEDED(retval)) {
        retval = this->write(commands_v2::calibration_ok);
    }

    if (SUCCEEDED(retval)) {
        retval = this->write(commands_v2::stream_mode);
    }

    return retval;
}


/*
 * powenetics_device::read
 */
void powenetics_device::read(void) {
    set_thread_name("powenetics sampler");

    std::vector<byte_type> buffer;
    buffer.resize(4 * 1024);
    DWORD read;

    while (::ReadFile(this->_handle,
            buffer.data(),
            static_cast<DWORD>(buffer.size()),
            &read,
            nullptr)) {



    }
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

    return HRESULT_FROM_WIN32(::GetLastError());

#else /* defined(_WIN32) */
    throw "TODO";
#endif /* defined(_WIN32) */
}
