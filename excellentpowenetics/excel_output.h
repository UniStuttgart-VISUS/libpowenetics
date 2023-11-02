// <copyright file="excel_output.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cassert>
#include <string>
#include <vector>

#include <Windows.h>

#include <libpowenetics/sample.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wil/result.h>


/// <summary>
/// Holds the results of processing the command line arguments.
/// </summary>
class excel_output final {

public:

    /// <summary>
    /// Initialises a new instance by opening Excel.
    /// </summary>
    excel_output(void);

    excel_output(const excel_output&) = delete;

    excel_output(excel_output&&) = default;

    /// <summary>
    /// Finalises the instance by closing Excel.
    /// </summary>
    ~excel_output(void) noexcept;

    /// <summary>
    /// Close Excel if it is open.
    /// </summary>
    void close(void);

    /// <summary>
    /// Open Excel if it is not yet open.
    /// </summary>
    void open(void);

    /// <summary>
    /// Save the workbook (at the specified path, if
    /// <paramref name="path" /> is not empty).
    /// </summary>
    void save(_In_ const std::wstring& path);

    /// <summary>
    /// Show or hide the Excel application window.
    /// </summary>
    /// <param name="visible"></param>
    /// <returns></returns>
    excel_output& visible(_In_ const bool visible);

    excel_output& operator =(const excel_output&) = delete;

    excel_output& operator =(excel_output&&) = default;

    /// <summary>
    /// Write the given sample into the Excel sheet.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    excel_output& operator <<(_In_ const powenetics_sample& rhs);

private:

    /// <summary>
    /// Get column name for zero-based (!) index 'col'.
    /// <summary>
    static std::wstring column_name(_In_ const long col);

    /// <summary>
    /// Invoke the specified verb on <paramref name="dispatch" />.
    /// </summary>
    /// <remarks>
    /// This is where all of the magic happens. This method converts the weird
    /// COM invokes into a somewhat passable C++ interface.
    /// </remarks>
    template<class... TArgs>
    static void invoke(_Inout_opt_ VARIANT *outResult,
        _In_ const int automationType,
        _In_ IDispatch *dispatch,
        _In_ LPOLESTR name,
        TArgs&&... args);

    /// <summary>
    /// Invoke the specified verb on <paramref name="dispatch" />.
    /// </summary>
    template<class TDispatch, class... TArgs>
    static void invoke(_Inout_opt_ VARIANT *outResult,
            _In_ const int automationType,
            _In_ const wil::com_ptr<TDispatch>& dispatch,
            _In_ LPOLESTR name,
            TArgs&&... args) {
        invoke(outResult, automationType, dispatch.get(), name,
            std::forward<TArgs>(args)...);
    }

    /// <summary>
    /// Get zero-based (!) cell range.
    /// <summary>
    wil::com_ptr<IDispatch> get_range(_In_ const long first_row,
        _In_ const long first_col, _In_ const long last_row,
        _In_ const long last_col);

    /// <summary>
    /// Get zero-based (!) cell range in a single row.
    /// <summary>
    inline wil::com_ptr<IDispatch> get_range(_In_ const long row,
            _In_ const long first_col, const long last_col) {
        return this->get_range(row, first_col, row, last_col);
    }

    /// <summary>
    /// Get zero-based (!) cell.
    /// <summary>
    inline wil::com_ptr<IDispatch> get_range(_In_ const long row,
            _In_ const long col) {
        return this->get_range(row, col, col);
    }

    /// <summary>
    /// Gets the used range in <see cref="_sheet" />.
    /// </summary>
    wil::com_ptr<IDispatch> get_used_range(void);

    /// <summary>
    /// Gets the zero-based (!) last row in the active sheet.
    /// </summary>
    long last_row(void);

    /// <summary>
    /// Reads the value from the specified cell.
    /// </summary>
    void read_value(_Out_ VARIANT& outValue, _In_ const long row,
        _In_ const long col);

    /// <summary>
    /// Writes a formula to the specified cell.
    /// </summary>
    void write_formula(_In_ const std::wstring& formula, _In_ const long row,
        _In_ const long col);

    /// <summary>
    /// Writes a value to the specified cell.
    /// </summary>
    void write_value(_In_ const wil::unique_variant& value, _In_ const long row,
        _In_ const long col);

    /// <summary>
    /// Writes a value to the specified cell.
    /// </summary>
    void write_value(_In_ const float value, _In_ const long row,
        _In_ const long col);

    /// <summary>
    /// Writes a value to the specified cell.
    /// </summary>
    void write_value(_In_ const std::uint16_t value, _In_ const long row,
        _In_ const long col);

    /// <summary>
    /// Writes a value to the specified cell.
    /// </summary>
    void write_value(_In_ const std::int64_t value, _In_ const long row,
        _In_ const long col);

    /// <summary>
    /// Writes a value to the specified cell.
    /// </summary>
    void write_value(_In_ const std::wstring& value, _In_ const long row,
        _In_ const long col);

    wil::com_ptr<IDispatch> _book;
    wil::com_ptr<IDispatch> _books;
    wil::com_ptr<IDispatch> _excel;
    wil::com_ptr<IDispatch> _sheet;
};

#include "excel_output.inl"
