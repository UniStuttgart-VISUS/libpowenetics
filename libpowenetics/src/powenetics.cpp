﻿// <copyright file="powenetics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "libpowenetics/powenetics.h"

#include <cassert>
#include <memory>

#include "device.h"


/*
 * ::powenetics_calibrate
 */
HRESULT powenetics_calibrate(_In_ const powenetics_handle handle) {
    return (handle == nullptr)
        ? E_HANDLE
        : handle->calibrate();
}


/*
 * ::powenetics_close
 */
HRESULT powenetics_close(_In_ const powenetics_handle handle) {
    if (handle == nullptr) {
        return E_HANDLE;
    } else {
        delete handle;
        return S_OK;
    }
}


/*
 * ::powenetics_open
 */
HRESULT powenetics_open(_Out_ powenetics_handle *out_handle,
        _In_z_ const powenetics_char *com_port,
        _In_ const powenetics_serial_configuration *config) {
    if ((out_handle == nullptr) || (com_port == nullptr)) {
        return E_INVALIDARG;
    }

    *out_handle = new (std::nothrow) powenetics_device();
    if (*out_handle == nullptr) {
        return E_OUTOFMEMORY;
    }

    auto retval = (**out_handle).open(com_port, config);
    if (retval != S_OK) {
        delete *out_handle;
        *out_handle = nullptr;
    }

    return retval;
}


/*
 * powenetics_reset_calibration
 */
HRESULT powenetics_reset_calibration(_In_ const powenetics_handle handle) {
    return (handle == nullptr)
        ? E_HANDLE
        : handle->reset_calibration();
}


/*
 * ::powenetics_start_streaming
 */
HRESULT powenetics_start_streaming(_In_ const powenetics_handle handle) {
    return (handle == nullptr)
        ? E_HANDLE
        : handle->start_streaming();
}