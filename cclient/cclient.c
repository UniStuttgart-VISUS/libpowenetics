// <copyright file="cclient.c" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <libpowenetics/powenetics.h>

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

    // Initialisation phase.
    if (SUCCEEDED(hr)) {
        hr = powenetics_open(&handle, argv[1], NULL);
    }

    if (SUCCEEDED(hr)) {
        hr = powenetics_calibrate(handle);
    }

    if (SUCCEEDED(hr)) {
        hr = powenetics_start_streaming(handle, on_sample, NULL);
    }

    // Cleanup phase.
    if (handle != NULL) {
        powenetics_close(handle);
    }

    return 0;
}
