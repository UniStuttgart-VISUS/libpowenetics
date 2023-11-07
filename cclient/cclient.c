// <copyright file="cclient.c" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <libpowenetics/powenetics.h>

#include <assert.h>

#if defined(_WIN32)
#include <Windows.h>
#include <tchar.h>
#else /* defined(_WIN32) */
#define _tmain main
#define _TCHAR char
#endif /* defined(_WIN32) */


/// <summary>
/// A callback to receive the samples from the device.
/// </summary>
/// <param name="src"></param>
/// <param name="sample"></param>
/// <param name="ctx"></param>
void on_sample(_In_ powenetics_handle src,
        _In_ const powenetics_sample *sample,
        _In_opt_ void *ctx) {

}


/// <summary>
/// The entry point of the test application for the C-style API.
/// </summary>
/// <remarks>
/// The main reason for this application existing at all is testing whether
/// it is actually working if linked against C code. We want to make this sure
/// in a test as we implement the library in C++ and only limit the interface
/// to C.
/// </remarks>
/// <param name="argc">The number of command line arguments.</param>
/// <param name="argv">The list of command line arguments.</param>
/// <returns>Zero, unconditionally.</returns>
int _tmain(int argc, _TCHAR **argv) {
    powenetics_serial_configuration config;
    powenetics_handle handle = NULL;
    HRESULT hr = S_OK;

    // Initialisation phase: either open the user-defined port or probe for one
    // Powenetics device attached to the machine.
    if (SUCCEEDED(hr)) {
        if (argc < 2) {
            size_t cnt = 1;
            hr = powenetics_probe(&handle, &cnt);

        } else {
            hr = powenetics_open(&handle, argv[1], NULL);
        }
    }
    assert((handle != NULL) || FAILED(hr));

    // Calibrate the device.
    if (SUCCEEDED(hr)) {
        hr = powenetics_calibrate(handle);
    }

    // Stream data to 'on_sample'.
    if (SUCCEEDED(hr)) {
        hr = powenetics_start_streaming(handle, on_sample, NULL);
    }

    // Cleanup phase.
    if (handle != NULL) {
        powenetics_close(handle);
    }

    return 0;
}
