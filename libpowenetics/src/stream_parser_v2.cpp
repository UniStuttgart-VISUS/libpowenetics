// <copyright file="stream_parser_v2.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "stream_parser_v2.h"


/*
 * stream_parser_v2::find_delimiter
 */
_Ret_maybenull_ const stream_parser_v2::byte_type *
stream_parser_v2::find_delimiter(_In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt) {
    if ((data == nullptr) || (cnt < delimiter.size())) {
        // Trivial reject.
        _powenetics_debug("Invalid input provided to find_delimiter.\r\n");
        return nullptr;
    }

    for (std::size_t i = 0; i < cnt; ++i) {
        // Note: This only works, because the delimiter is two bytes. If this
        // should ever change, we must check in a loop.
        assert(delimiter.size() == 2);
        if ((data[i] == delimiter.front())
                && (i + 1 < cnt)
                && (data[i + 1] == delimiter.back())) {
            return data + i;
        }
    }
    // Not found at this point.

    return nullptr;
}


/*
 * stream_parser_v2::parse_value
 */
powenetics_voltage_current stream_parser_v2::parse_value(
        _In_reads_(5) _Out_ const byte_type *& data,
        _In_ const float discard_threshold) noexcept {
    powenetics_voltage_current retval;

    retval.voltage = to_uint16(data) / 1000.0f;
    data += sizeof(std::uint16_t);

    if (retval.voltage > discard_threshold) {
        retval.current = to_uint24(data) / 1000.0f;
        data += 3;
    } else {
        retval.current = 0.0f;
    }

    return retval;
}
