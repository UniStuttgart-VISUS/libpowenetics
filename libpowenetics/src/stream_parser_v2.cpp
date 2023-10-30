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
        return nullptr;
    }

    for (std::size_t i = 0; i < cnt; ++i) {
        // Note: This only works, because the delimiter is two bytes. If this
        // should ever change, we must check in a loop.
        assert(delimiter.size() == 2);
        if ((data[i] == delimiter.front())
                && (i < cnt - 1)
                && (data[i + 1] == delimiter.back())) {
            return data + i;
        }
    }
    // Not found at this point.

    return nullptr;
}


/*
 * stream_parser_v2::to_uint16
 */
std::uint16_t stream_parser_v2::to_uint16(_In_reads_(2) const byte_type *src) {
    std::uint16_t retval;
    auto dst = reinterpret_cast<byte_type *>(&retval);
    dst[0] = src[1];
    dst[1] = src[0];
    return retval;
}
