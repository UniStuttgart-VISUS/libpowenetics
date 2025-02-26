// <copyright file="excel_output.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "excel_output.h"

#include <sstream>


/*
 * excel_output::excel_output
 */
excel_output::excel_output(void) {
    this->open();
}


/*
 * excel_output::excel_output
 */
excel_output::~excel_output(void) { }


/*
 * excel_output::close
 */
void excel_output::close(void) {
    if (this->_excel != nullptr) {
        //// Set .Saved property of workbook to TRUE so we aren't prompted
        //// to save when we tell Excel to quit...
        //{
        //    VARIANT x;
        //    x.vt = VT_I4;
        //    x.lVal = 1;
        //    AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlBook, L"Saved", 1, x);
        //}

        invoke(NULL, DISPATCH_METHOD, this->_excel, L"Quit");

        this->_book.reset();
        this->_books.reset();
        this->_excel.reset();
        this->_sheet.reset();
    }
}


/*
 * excel_output::open
 */
void excel_output::open(void) {
    if (this->_excel == nullptr) {
        CLSID clsid;                // CLSID of Excel application.

        // Get CLSID of Excel.
        THROW_IF_FAILED(::CLSIDFromProgID(L"Excel.Application", &clsid));

        // Start Excel server.
        THROW_IF_FAILED(::CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER,
            IID_IDispatch, this->_excel.put_void()));

        // Get Workbooks collection.
        {
            wil::unique_variant output;
            invoke(&output, DISPATCH_PROPERTYGET, this->_excel, L"Workbooks");
            this->_books = output.pdispVal;
        }

        // Add a new workbook for the results.
        {
            wil::unique_variant output;
            invoke(&output, DISPATCH_PROPERTYGET, this->_books, L"Add");
            this->_book = output.pdispVal;
        }

        // Get the active sheet.
        {
            wil::unique_variant output;
            invoke(&output, DISPATCH_PROPERTYGET, this->_excel, L"ActiveSheet");
            this->_sheet = output.pdispVal;
        }
    }
}


/*
 * excel_output::save
 */
void excel_output::save(_In_ const std::wstring& path) {
    if (path.empty()) {
        invoke(nullptr, DISPATCH_METHOD, this->_book, L"Save");

    } else {
        wil::unique_variant input;
        input.vt = VT_BSTR;
        input.bstrVal = ::SysAllocString(path.c_str());
        invoke(nullptr, DISPATCH_METHOD, this->_book, L"SaveAs", input);
    }
}


/*
 * excel_output::visible
 */
excel_output& excel_output::visible(_In_ const bool visible) {
    wil::unique_variant input;
    input.vt = VT_I4;
    input.lVal = visible ? 1 : 0;
    invoke(nullptr, DISPATCH_PROPERTYPUT, this->_excel, L"Visible", input);
    return *this;
}


/*
 * excel_output::operator <<
 */
excel_output& excel_output::operator <<(_In_ const powenetics_sample& rhs) {
    if (this->_sheet == nullptr) {
        THROW_IF_FAILED(E_NOT_VALID_STATE);
    }

    auto row = this->last_row();
    long col = 0, col_i, col_u;

    // If this is the first row, write the header.
    if (row == 0) {
        this->write_value(L"Timestamp", row, col++);
        this->write_value(L"Sequence Number", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"ATX 12V [V]", row, col_u);
        this->write_value(L"ATX 12V [A]", row, col_i);
        this->write_value(L"ATX 12V [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"ATX 3.3V [V]", row, col_u);
        this->write_value(L"ATX 3.3V [A]", row, col_i);
        this->write_value(L"ATX 3.3V [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"ATX 5V [V]", row, col_u);
        this->write_value(L"ATX 5V [A]", row, col_i);
        this->write_value(L"ATX 5V [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"ATX STB [V]", row, col_u);
        this->write_value(L"ATX STB [A]", row, col_i);
        this->write_value(L"ATX STB [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"EPS #1 [V]", row, col_u);
        this->write_value(L"EPS #1 [A]", row, col_i);
        this->write_value(L"EPS #1 [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"EPS #2 [V]", row, col_u);
        this->write_value(L"EPS #2 [A]", row, col_i);
        this->write_value(L"EPS #2 [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"EPS #3 [V]", row, col_u);
        this->write_value(L"EPS #3 [A]", row, col_i);
        this->write_value(L"EPS #3 [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"PCIe 12V #1 [V]", row, col_u);
        this->write_value(L"PCIe 12V #1 [A]", row, col_i);
        this->write_value(L"PCIe 12V #1 [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"PCIe 12V #2 [V]", row, col_u);
        this->write_value(L"PCIe 12V #2 [A]", row, col_i);
        this->write_value(L"PCIe 12V #2 [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"PCIe 12V #3 [V]", row, col_u);
        this->write_value(L"PCIe 12V #3 [A]", row, col_i);
        this->write_value(L"PCIe 12V #3 [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"PEG 12V [V]", row, col_u);
        this->write_value(L"PEG 12V [A]", row, col_i);
        this->write_value(L"PEG 12V [W]", row, col++);

        col_u = col++;
        col_i = col++;
        this->write_value(L"PEG 3.3V [V]", row, col_u);
        this->write_value(L"PEG 3.3V [A]", row, col_i);
        this->write_value(L"PEG 3.3V [A]", row, col++);

        ++row;
        col = 0;
    } /* if (row == 0) */

    this->write_value(rhs.timestamp, row, col++);
    this->write_value(rhs.sequence_number, row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.atx_12v.voltage, row, col_u);
    this->write_value(rhs.atx_12v.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.atx_3_3v.voltage, row, col_u);
    this->write_value(rhs.atx_3_3v.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.atx_5v.voltage, row, col_u);
    this->write_value(rhs.atx_5v.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.atx_stb.voltage, row, col_u);
    this->write_value(rhs.atx_stb.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.eps1.voltage, row, col_u);
    this->write_value(rhs.eps1.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.eps2.voltage, row, col_u);
    this->write_value(rhs.eps2.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.eps3.voltage, row, col_u);
    this->write_value(rhs.eps3.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.pcie_12v1.voltage, row, col_u);
    this->write_value(rhs.pcie_12v1.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.pcie_12v2.voltage, row, col_u);
    this->write_value(rhs.pcie_12v2.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.pcie_12v3.voltage, row, col_u);
    this->write_value(rhs.pcie_12v3.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.peg_12v.voltage, row, col_u);
    this->write_value(rhs.peg_12v.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    col_u = col++;
    col_i = col++;
    this->write_value(rhs.peg_3_3v.voltage, row, col_u);
    this->write_value(rhs.peg_3_3v.current, row, col_i);
    this->write_formula(cell_name(row, col_u) + L"*"
        + cell_name(row, col_u), row, col++);

    return *this;
}


/*
 * excel_output::cell_name
 */
std::wstring excel_output::cell_name(_In_ const long row, _In_ const long col) {
    return column_name(col) + std::to_wstring(row + 1);
}


/*
 * excel_output::column_name
 */
std::wstring excel_output::column_name(_In_ const long col) {
    const long RANGE = L'Z' - L'A' + 1;
    long d = col + 1;
    long r = 0;
    std::wstring retval;

    while (d > 0) {
        r = (d - 1) % RANGE;
        retval = std::wstring(1, (L'A' + static_cast<wchar_t>(r))) + retval;
        d = (d - r) / RANGE;
    }

    return retval;
}


/*
 * excel_output::get_range
 */
wil::com_ptr<IDispatch> excel_output::get_range(_In_ const long first_row,
        _In_ const long first_col, _In_ const long last_row,
        _In_ const long last_col) {
    wil::unique_variant input;      // Input of automation call.
    wil::unique_variant output;     // Output of automation call.
    std::wstringstream range_spec;  // String representation of the retval.
    wil::com_ptr<IDispatch> retval; // Range of new row.

    // Prepare input.
    range_spec << excel_output::column_name(first_col) << (first_row + 1)
        << L":" << excel_output::column_name(last_col) << (last_row + 1)
        << std::ends;
    input.vt = VT_BSTR;
    input.bstrVal = ::SysAllocString(range_spec.str().c_str());

    // Make the call.
    assert(this->_sheet != nullptr);
    invoke(&output, DISPATCH_PROPERTYGET, this->_sheet, L"Range", input);
    retval = output.pdispVal;

    return retval;
}


/*
 * excel_output::get_used_range
 */
wil::com_ptr<IDispatch> excel_output::get_used_range(void) {
    wil::unique_variant output;
    assert(this->_sheet != nullptr);
    invoke(&output, DISPATCH_PROPERTYGET, this->_sheet, L"UsedRange");
    wil::com_ptr<IDispatch> retval(output.pdispVal);
    return retval;
}


/*
 * excel_output::last_row
 */
long excel_output::last_row(void) {
    // Get the used retval.
    auto range = this->get_used_range();

    // Retrieve the used rows.
    wil::unique_variant rows;
    invoke(&rows, DISPATCH_PROPERTYGET, range, L"Rows");
    range = rows.pdispVal;

    // Retrieve number of rows.
    wil::unique_variant count;
    invoke(&count, DISPATCH_PROPERTYGET, range, L"Count");
    auto retval = count.lVal - 1;

    return retval;
}


/*
 * excel_output::read_value
 */
void excel_output::read_value(_Out_ VARIANT& outValue,
        _In_ const long row, _In_ const long col) {
    // Get the retval to read from.
    auto range = this->get_range(row, col);
    assert(range != nullptr);

    // Get the value.
    ::VariantInit(&outValue);
    invoke(&outValue, DISPATCH_PROPERTYGET, range, L"Value");
}


/*
 * excel_output::write_formula
 */
void excel_output::write_formula(_In_ const std::wstring& formula,
        _In_ const long row, _In_ const long col) {

    // Prepare input.
    wil::unique_variant cell;
    cell.vt = VT_BSTR;
    cell.bstrVal = ::SysAllocString(formula.c_str());

    // Get the retval we write to.
    auto range = this->get_range(row, col);
    assert(range != nullptr);

    // Write the formula.
    invoke(nullptr, DISPATCH_PROPERTYPUT, range, L"Formula", cell);
}


/*
 * excel_output::write_value
 */
void excel_output::write_value(_In_ const wil::unique_variant& value,
        _In_ const long row, _In_ const long col) {
    // Get the cell we want to write to.
    auto range = this->get_range(row, col);
    assert(range != nullptr);

    // Write the property.
    invoke(nullptr, DISPATCH_PROPERTYPUT, range, L"Value", value);
}


/*
 * excel_output::write_value
 */
void excel_output::write_value(_In_ const float value, _In_ const long row,
        _In_ const long col) {
    wil::unique_variant v;
    v.vt = VT_R4;
    v.fltVal = value;
    this->write_value(v, row, col);
}


/*
 * excel_output::write_value
 */
void excel_output::write_value(_In_ const std::uint16_t value,
        _In_ const long row, _In_ const long col) {
    wil::unique_variant v;
    v.vt = VT_UI2;
    v.uiVal = value;
    this->write_value(v, row, col);
}


/*
 * excel_output::write_value
 */
void excel_output::write_value(_In_ const std::int64_t value,
        _In_ const long row, _In_ const long col) {
    wil::unique_variant v;
    v.vt = VT_I8;
    v.llVal = value;
    this->write_value(v, row, col);
}


/*
 * excel_output::write_value
 */
void excel_output::write_value(_In_ const std::wstring& value,
        _In_ const long row, _In_ const long col) {
    wil::unique_variant v;
    v.vt = VT_BSTR;
    v.bstrVal = ::SysAllocString(value.c_str());
    this->write_value(v, row, col);
}

#if 0
/*
 * excel_output::write_value
 */
void excel_output::write_value(const variant& value, const long row,
        const long col) {
    VARIANT inputCell;
    std::stringstream ss;

    /* Prepare input. */
    ::VariantInit(&inputCell);
    // TODO: this is suboptimal. Find a way to template VARIANT, too.
    switch (value.type()) {
        case variant_type::boolean:
            inputCell.vt = VT_BOOL;
            inputCell.boolVal = value.get<variant_type::boolean>();
            break;

        case variant_type::int8:
            inputCell.vt = VT_I1;
            inputCell.cVal = value.get<variant_type::int8>();
            break;

        case variant_type::int16:
            inputCell.vt = VT_I2;
            inputCell.iVal = value.get<variant_type::int16>();
            break;

        case variant_type::int32:
            inputCell.vt = VT_I4;
            inputCell.intVal = value.get<variant_type::int32>();
            break;

        case variant_type::int64:
            inputCell.vt = VT_I8;
            inputCell.llVal = value.get<variant_type::int64>();
            break;

        case variant_type::uint8:
            inputCell.vt = VT_UI1;
            inputCell.bVal = value.get<variant_type::uint8>();
            break;

        case variant_type::uint16:
            inputCell.vt = VT_UI2;
            inputCell.uiVal = value.get<variant_type::uint16>();
            break;

        case variant_type::uint32:
            inputCell.vt = VT_UI4;
            inputCell.uintVal = value.get<variant_type::uint32>();
            break;

        case variant_type::uint64:
            inputCell.vt = VT_UI8;
            inputCell.ullVal = value.get<variant_type::uint64>();
            break;

        case variant_type::float32:
            inputCell.vt = VT_R4;
            inputCell.fltVal = value.get<variant_type::float32>();
            break;

        case variant_type::float64:
            inputCell.vt = VT_R8;
            inputCell.dblVal = value.get<variant_type::float64>();
            break;

        case variant_type::string:
            inputCell.vt = VT_BSTR;
            inputCell.bstrVal = ::SysAllocString(excel_output::convert(
                value.get<variant_type::string>()).c_str());
            break;

        case variant_type::wstring:
            inputCell.vt = VT_BSTR;
            inputCell.bstrVal = ::SysAllocString(
                value.get<variant_type::wstring>().c_str());
            break;

        case variant_type::device:
            inputCell.vt = VT_BSTR;
            ss.clear();
            ss << value.get<variant_type::device>() << std::ends;
            inputCell.bstrVal = ::SysAllocString(excel_output::convert(
                ss.str()).c_str());
            break;

        case variant_type::environment:
            inputCell.vt = VT_BSTR;
            ss.clear();
            ss << value.get<variant_type::environment>() << std::ends;
            inputCell.bstrVal = ::SysAllocString(excel_output::convert(
                ss.str()).c_str());
            break;
    }

    /* Get the retval we write to. */
    auto range = this->get_range(row, col);
    assert(range != nullptr);

    /* Set retval to 'inputRow'. */
    try {
        excel_output::invoke(nullptr, DISPATCH_PROPERTYPUT, range, L"Value",
            inputCell);
        ::VariantClear(&inputCell);
    } catch (...) {
        ::VariantClear(&inputCell);
        throw;
    }
}
#endif
