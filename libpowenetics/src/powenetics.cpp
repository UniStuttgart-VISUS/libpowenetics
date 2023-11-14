// <copyright file="powenetics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "libpowenetics/powenetics.h"

#include <algorithm>
#include <cassert>
#include <memory>

#include "debug.h"
#include "device.h"
#include "thread_name.h"


/*
 * ::powenetics_calibrate
 */
HRESULT powenetics_calibrate(_In_ const powenetics_handle handle,
        _In_ const uint8_t channel,
        _In_ const powenetics_quantity quantity,
        _In_ const uint32_t value) {
    return (handle == nullptr)
        ? E_HANDLE
        : handle->calibrate(channel, quantity, value);
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
        _In_opt_ const powenetics_serial_configuration *config) {
    if (out_handle == nullptr) {
        _powenetics_debug("Invalid storage location for handle provided.\r\n");
        return E_INVALIDARG;
    }
    if (com_port == nullptr) {
        _powenetics_debug("Invalid COM port provided.\r\n");
        return E_INVALIDARG;
    }

    powenetics_serial_configuration dft_conf;
    if (config == nullptr) {
        dft_conf.version = 2;
        auto retval = powenetics_initialise_serial_configuration(&dft_conf);
        if (retval != S_OK) {
            _powenetics_debug("Failed to initialise default serial "
                "configuration.\r\n");
            return retval;
        }
    }

    std::unique_ptr<powenetics_device> device(
        new (std::nothrow) powenetics_device());
    if (device == nullptr) {
        _powenetics_debug("Insufficient memory for powenetics_device.\r\n");
        return E_OUTOFMEMORY;
    }

    auto conf = (config != nullptr) ? config : &dft_conf;
    auto retval = device->open(com_port, conf);
    if (retval != S_OK) {
        _powenetics_debug("Failed to open powenetics_device.\r\n");
    }

    // If everything went OK so far, detach from unique_ptr.
    *out_handle = device.release();

    return retval;
}


/*
 * ::powenetics_probe
 */
HRESULT LIBPOWENETICS_API powenetics_probe(
        _Out_writes_opt_(*cnt) powenetics_handle *out_handles,
        _Inout_ size_t *cnt) {
    if (cnt == nullptr) {
        _powenetics_debug("A valid number of handles must be provided.\r\n");
        return E_POINTER;
    }

    // Ensure that the counter is never valid if the output buffer is invalid.
    if (out_handles == nullptr) {
        *cnt = 0;
    }

    // Get all serial ports as candidates where a device could be attached to.
    auto candidates = powenetics_device::probe_candidates();

    // Check all serial ports by asynchronously opening a device on it.
    if (!candidates.empty()) {
        powenetics_serial_configuration config;
        config.version = 2;
        {
            auto hr = ::powenetics_initialise_serial_configuration(&config);
            if (FAILED(hr)) {
                return hr;
            }
        }

        // Determine how many threads we start to probe the COM ports. As we
        // expect many of them to fail with a timeout, we try probing them in
        // parallel to reduce the time required.
        const std::size_t max_threads = std::thread::hardware_concurrency();
        const auto cnt_threads = (std::min)(candidates.size(), max_threads);

        std::atomic<std::size_t> cur_candidate(0);
        std::atomic<std::size_t> cur_ouput(0);
        std::vector<std::unique_ptr<powenetics_device>> devices(
            candidates.size());
        std::vector<std::thread> threads(cnt_threads);

        for (auto& t : threads) {
            t = std::thread([&](void) {
                ::set_thread_name("libpowenetics probing thread");
                auto mine = cur_candidate++;

                while (mine < candidates.size()) {
                    // Try opening the candidate and erase it from the list of
                    // candidates if the operation failed.
                    std::unique_ptr<powenetics_device> d(
                        new (std::nothrow) powenetics_device());

                    auto hr = (d != nullptr) ? S_OK : E_OUTOFMEMORY;

                    // TODO: We need to set some timeout here.
                    if (SUCCEEDED(hr)) {
                        hr = d->open(candidates[mine].c_str(), &config);
                    }

                    // TODO: what would we do for testing here?
                    //if (SUCCEEDED(hr)) {
                    //    hr = d->calibrate();
                    //}

                    if (SUCCEEDED(hr)) {
                        devices[mine] = std::move(d);
                    }

                    mine = cur_candidate++;
                }
            });
        }

        // Wait for all the tester threads to exit.
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        // Count how many devices we have found and report this to 'cnt'.
        devices.erase(std::remove_if(devices.begin(), devices.end(),
            [](const std::unique_ptr<powenetics_device>& d) { return !d; }),
            devices.end());

        // Return as much as we can. All handles we cannot return will be closed
        // as the local variable gets destroyed.
        for (std::size_t i = 0; (i < *cnt) && (i < devices.size()); ++i) {
            out_handles[i] = devices[i].release();
        }

        auto retval = (candidates.size() <= *cnt)
            ? S_OK
            : HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        if ((*cnt = candidates.size()) < 1) {
            retval = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        return retval;

    } else {
        // Nothing found, which is an error in any case.
        *cnt = 0;
        return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
    }

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
HRESULT powenetics_start_streaming(_In_ const powenetics_handle handle,
        _In_ const powenetics_data_callback callback,
        _In_opt_ void *context) {
    return (handle == nullptr)
        ? E_HANDLE
        : handle->start(callback, context);
}


/*
 * ::powenetics_stop_streaming
 */
HRESULT powenetics_stop_streaming(_In_ const powenetics_handle handle) {
    return (handle == nullptr)
        ? E_HANDLE
        : handle->stop();
}
