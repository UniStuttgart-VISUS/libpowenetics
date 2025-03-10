﻿// <copyright file="cclient.c" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <libpowenetics/powenetics.h>

#include <assert.h>
#include <stdio.h>

#if defined(_WIN32)
#include <conio.h>
#include <Windows.h>
#include <tchar.h>
#else /* defined(_WIN32) */
#include <unistd.h>

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
    printf("ATX 12V: %f V, %f A\r\n",
        sample->atx_12v.voltage,
        sample->atx_12v.current);
    printf("ATX 3.3V: %f V, %f A\r\n",
        sample->atx_3_3v.voltage,
        sample->atx_3_3v.current);
    printf("ATX 5V: %f V, %f A\r\n",
        sample->atx_5v.voltage,
        sample->atx_5v.current);
    printf("ATX Stand-by: %f V, %f A\r\n",
        sample->atx_stb.voltage,
        sample->atx_stb.current);
    printf("EPS #1: %f V, %f A\r\n",
        sample->eps1.voltage,
        sample->eps1.current);
    printf("EPS #2: %f V, %f A\r\n",
        sample->eps2.voltage,
        sample->eps2.current);
    printf("EPS #3: %f V, %f A\r\n",
        sample->eps3.voltage,
        sample->eps3.current);
    printf("PCIe 12V #1: %f V, %f A\r\n",
        sample->pcie_12v1.voltage,
        sample->pcie_12v1.current);
    printf("PCIe 12V #2: %f V, %f A\r\n",
        sample->pcie_12v2.voltage,
        sample->pcie_12v2.current);
    printf("PCIe 12V #3: %f V, %f A\r\n",
        sample->pcie_12v3.voltage,
        sample->pcie_12v3.current);
    printf("PEG 12V: %f V, %f A\r\n",
        sample->peg_12v.voltage,
        sample->peg_12v.current);
    printf("PEG 3.3V: %f V, %f A\r\n",
        sample->peg_3_3v.voltage,
        sample->peg_3_3v.current);
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
            size_t cnt = 0;
            powenetics_char *paths = NULL;

            hr = powenetics_probe(paths, &cnt);
            if (hr == HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)) {
                paths = malloc(cnt * sizeof(powenetics_char));
                hr = (paths == NULL) ? E_OUTOFMEMORY : S_OK;
            }

            if (SUCCEEDED(hr)) {
                assert(paths != NULL);
                hr = powenetics_probe(paths, &cnt);
            }

            if (SUCCEEDED(hr)) {
                hr = powenetics_open(&handle, paths, NULL);
            }

            if (paths != NULL) {
                free(paths);
            }

        } else {
            hr = powenetics_open(&handle, argv[1], NULL);
        }
    }
    assert((handle != NULL) || FAILED(hr));

    // Calibrate the device.
    if (SUCCEEDED(hr)) {
        //hr = powenetics_calibrate(handle);
    }

    // Stream data to 'on_sample'.
    if (SUCCEEDED(hr)) {
        hr = powenetics_start_streaming(handle, on_sample, NULL);
    }

    if (SUCCEEDED(hr)) {
#if defined(_WIN32)
        printf("Press any key to end measurement.\n");
        getch();
#else /* defined(_WIN32) */
        sleep(10);
#endif /* defined(_WIN32) */
    }

    // Cleanup phase.
    if (handle != NULL) {
        powenetics_close(handle);
    }

    return 0;
}
