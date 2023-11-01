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
    auto& delimiter = responses_v2::segment_delimiter;

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
 * stream_parser_v2::parse_segment
 */
bool stream_parser_v2::parse_segment(
        _Out_ powenetics_sample& dst,
        _In_reads_(end - begin) const byte_type *begin,
        _In_ const byte_type *end) noexcept {
    assert(begin != nullptr);
    assert(end != nullptr);
    assert(end > begin);
    const auto retval = (end - begin) == segment_length;

    if (retval) {
        dst.version = 2;
        dst.sequence_number = to_uint16(begin);
        dst.timestamp = ::powenetics_make_timestamp();
        // Note: The original implementation performs down-sampling on user
        // request at this point. We do not do that in the library. Instead, the
        // user must do that in the callback if it is desired.

        // The samples following the sequence number comprise 16 bits of voltage
        // data followed by 24 bit of current data each.
        auto cur = begin + sizeof(std::uint16_t);

        // Channel 1: ATX 3.3V. The ATX 10-pin connector does not have that,
        // so parse_value will discard set the current to zero if a voltage
        // lower than 1 is reported.
        dst.atx_3_3v = parse_value(cur);
        assert(cur < end);

        // Channel 2: ATX 5V STB, only on 24-pin connectors. For 10-pin
        // connectors, this will be overwritten by channel 6 later.
        dst.atx_stb = parse_value(cur);
        assert(cur < end);

        // Channel 3: ATX 12V
        dst.atx_12v = parse_value(cur);
        assert(cur < end);

        // Channel 4: ATX 5V
        dst.atx_5v = parse_value(cur);
        assert(cur < end);

        // Channel 5: EPS #1
        dst.eps1 = parse_value(cur);
        assert(cur < end);

        // Channel 6: ATX 12V STB, only on 10-pin connectors. Note that the
        // value still must be parsed to advance 'cur'!
        {
            auto s = parse_value(cur);
            if (dst.atx_3_3v.voltage < 1.0f) {
                dst.atx_stb = s;
            }
        }
        assert(cur < end);

        // Channel 7: EPS #3
        dst.eps3 = parse_value(cur);
        assert(cur < end);

        // Channel 8: EPS #2
        dst.eps2 = parse_value(cur);
        assert(cur < end);

        // Channel 9: PCIe #3
        dst.pcie_12v3 = parse_value(cur);
        assert(cur < end);

        // Channel 10: PCIe #2
        dst.pcie_12v2 = parse_value(cur);
        assert(cur < end);

        // Channel 11: PEG 3.3V
        dst.peg_3_3v = parse_value(cur);
        assert(cur < end);

        // Channel 12: PEG 12V
        dst.peg_12v = parse_value(cur);
        assert(cur < end);

        // Channel 13: PCIe #1
        dst.pcie_12v1 = parse_value(cur);
        assert(cur == end);
    }

    return retval;
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
