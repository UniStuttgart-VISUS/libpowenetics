// <copyright file="excellentpowenetics.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <iostream>

#include <Windows.h>
#include <tchar.h>

#include <libpowenetics/powenetics.h>

#include <wil/result.h>

#include "cmd_line.h"
#include "excel_output.h"


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
    try {
        cmd_line cmd_line(argc, argv);
        auto com_scope = wil::CoInitializeEx();

        excel_output excel;
        excel.visible(cmd_line.excel_visible());

        powenetics_sample s;
        excel << s;



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
