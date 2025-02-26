// <copyright file="excellentpowenetics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <iostream>
#include <thread>

#include <conio.h>
#include <Windows.h>
#include <tchar.h>

#include <libpowenetics/powenetics.h>

#include <wil/result.h>

#include "cmd_line.h"
#include "excel_output.h"
#include "excel_worker.h"


/// <summary>
/// The entry point of the application.
/// </summary>
/// <remarks>
/// This demo application shows how to use libpowenetics in C++. It connects
/// to a Powenetics v2 power measurement kit and writes the data it receives
/// from there into an Excel sheet.
/// </remarks>
/// <param name="argc">The number of command line arguments.</param>
/// <param name="argv">The list of command line arguments.</param>
/// <returns>Zero, unconditionally.</returns>
int wmain(_In_ const int argc, _In_reads_(argc) const wchar_t **argv) {
    typedef wil::unique_any<powenetics_handle, decltype(::powenetics_close),
        &::powenetics_close> unique_powenetics;

    try {
        cmd_line cmd_line(argc, argv);
        auto com_scope = wil::CoInitializeEx(COINIT_MULTITHREADED);

        // Configure the Excel output.
        excel_output output;
        output.visible(cmd_line.excel_visible());

        // Open the Powenetics device.
        unique_powenetics powenetics;
        THROW_IF_FAILED(::powenetics_open(powenetics.put(), L"", nullptr));

        // Start the worker and wait for the user to end measurement.
        excel_worker worker(powenetics.release(), output);
        std::cout << "Press any key to end measurement." << std::endl;
        ::getch();
        worker.stop();

        // If the user provided an output path, save the Excel workbook and
        // close Excel. Otherwise, we leave it running and let the user decide
        // what to do.
        if (!cmd_line.output_path().empty()) {
            output.save(cmd_line.output_path());
            output.close();
        }

        return 0;

    } catch (wil::ResultException ex) {
        std::cout << ex.what() << std::endl;
        return ex.GetErrorCode();

    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return -1;

    } catch (...) {
        std::cout << "Unexpected exception caught at root level" << std::endl;
        return -2;
    }
}
