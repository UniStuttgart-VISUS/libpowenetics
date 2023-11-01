// <copyright file="cmd_line.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <string>

#include <Windows.h>


/// <summary>
/// Holds the results of processing the command line arguments.
/// </summary>
class cmd_line final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="argc"></param>
    /// <param name="argv"></param>
    cmd_line(_In_ const int argc, _In_reads_(argc) const wchar_t **argv);

    /// <summary>
    /// Answer whether the Excel window should be visible.
    /// </summary>
    /// <returns></returns>
    inline bool excel_visible(void) const noexcept {
        return (this->_excel_visible || this->_output_path.empty());
    }

    /// <summary>
    /// Answer the path where the Excel sheet should be saved.
    /// </summary>
    /// <returns>The path where the output should be saved. If this is empty,
    /// the output is not saved automatically and Excel is forced to be visible
    /// and will not be closed by the application on exit.</returns>
    inline const std::wstring& output_path(void) const noexcept {
        return this->_output_path;
    }

    /// <summary>
    /// Answer the path to the serial port the Powenetics kit is connected to.
    /// </summary>
    /// <returns></returns>
    inline const std::wstring& serial_port(void) const noexcept {
        return this->_serial_port;
    }

private:

    bool _excel_visible;
    std::wstring _output_path;
    std::wstring _serial_port;

};
