// <copyright file="cmd_line.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "cmd_line.h"

#include <algorithm>
#include <cassert>


static bool equals(const wchar_t *lhs, const wchar_t *rhs) {
    auto llen = ::wcslen(lhs);
    auto rlen = ::wcslen(rhs);
    return ((llen == rlen) && std::equal(lhs, lhs + llen, rhs,
            [](const wchar_t l, const wchar_t r) {
        return (std::tolower(l) == std::tolower(r));
    }));
}


template<class TIterator>
static TIterator find_switch(TIterator begin, TIterator end,
        const wchar_t *name) {
    assert(name != nullptr);
    return std::find_if(begin, end, [name](const wchar_t *s) {
        return equals(s, name);
    });
}


template<class TIterator>
static TIterator find_argument(TIterator begin, TIterator end,
        const wchar_t *name) {
    auto retval = ::find_switch(begin, end, name);
    return (retval != end) ? ++retval : retval;
}


static bool starts_with(const wchar_t *haystack, const wchar_t *needle) {
    assert(haystack != nullptr);
    assert(needle != nullptr);
    auto pos = ::wcsstr(haystack, needle);
    return (pos == haystack);
}



/*
 * cmd_line::cmd_line
 */
cmd_line::cmd_line(_In_ const int argc, _In_reads_(argc) const wchar_t **argv)
        : _excel_visible(true) {
    const auto begin = argv;
    const auto end = argv + argc;

    {
        auto it = ::find_argument(begin, end, L"/port");
        if (it != end) {
            static const std::wstring abs_path(L"\\\\.\\");
            if (starts_with(*it, abs_path.c_str())) {
                this->_serial_port = *it;
            } else {
                this->_serial_port = abs_path + *it;
            }
        }
    }

    {
        auto it = ::find_argument(begin, end, L"/output");
        if (it != end) {
            this->_output_path = *it;
        }
    }

    {
        auto it = ::find_switch(begin, end, L"/visible");
        this->_excel_visible = (it != end);
    }
}
