// <copyright file="stream_parser_v2.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>


/*
 * stream_parser_v2::tokenise
 */
template<class TCallback>
bool stream_parser_v2::tokenise(_In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt,
        _In_ TCallback&& callback) {
    assert(data != nullptr);

    if (this->_buffer.empty()) {
        // If the buffer is empty, we first check the input in-place without
        // buffering it. Whatever is not part of a segment will be pushed to
        // the _buffer in the end and preserved for the next call.
        auto cur = find_delimiter(data, cnt);

        if (cur == nullptr) {
            // If we did not find the delimiter, we need to buffer everything
            // until the next call hopefully provides enough data. This should
            // only happen if too few data have been passed to the method to
            // find the delimiter.
            assert(this->_buffer.empty());
            assert(cnt < delimiter.size());
            this->_buffer.reserve(cnt);
            std::copy(data, data + cnt, std::back_inserter(this->_buffer));

        } else {
            // If buffer is empty and we found the separator, it must be at
            // the begin of the data, otherwise the stream is corrupted,
            // because we have no way to find the begin of the first segment
            // to deliver.
            assert(cur == data);

            while (cur != nullptr) {
                assert(cur[0] == delimiter.front());
                assert(cur[1] == delimiter.back());
                auto next = find_delimiter(cur + delimiter.size(), data - cur);

                if (next != nullptr) {
                    // We found a segment between 'cur' and 'next', so deliver
                    // it to the callback.
                    callback(cur, next);

                    assert(next[0] == delimiter.front());
                    assert(next[1] == delimiter.back());
                    next += delimiter.size();

                } else {
                    // We did not find the end of the segment after 'cur', so
                    // preserve the input until the next call.
                    this->_buffer.reserve(cnt);
                    std::copy(cur, data + cnt,
                        std::back_inserter(this->_buffer));
                }
            } /* while (cur != nullptr) */

            cur = next;
        } /* if (cur == nullptr) */

        return !this->_buffer.empty();

    } else {
        // If the buffer is not empty, we take it and call the method
        // recursively to parse the concatenated content. This is not the
        // most performant implementation, but it should suffice for now.
        auto buffer = std::move(this->_buffer);
        buffer.reserve(buffer.size() + cnt);
        std::copy(data, data + cnt, std::back_inserter(buffer));
        assert(this->_buffer.empty());
        return this->tokenise(buffer.data(), buffer.size(),
            std::forward(callback));
    }
}


/*
 * stream_parser_v2::parse_segment
 */
template<class TCallback>
bool stream_parser_v2::parse_segment(
        _In_reads_(end - begin) const byte_type *begin,
        _In_ const byte_type *end,
        _In_ TCallback&& callback) {
    assert(begin != nullptr);
    assert(end != nullptr);
    assert(end > begin);
    assert(static_cast<bool>(callback));
    const auto retval = (end - begin) == segment_length;

    if (retval) {
        powenetics_sample sample;
        sample.version = 2;
        sample.sequence_number = to_uint16(begin);
        // Note: The original implementation performs down-sampling on user
        // request at this point. We do not do that in the library. Instead, the
        // user must do that in the callback if it is desired.

        // The samples following the sequence number comprise 16 bits of voltage
        // data followed by 24 bit of current data each.
        auto cur = begin + sizeof(std::uint16_t);

        // Channel 1: ATX 3.3V. The ATX 10-pin connector does not have that,
        // so parse_value will discard set the current to zero if a voltage
        // lower than 1 is reported.
        sample.atx_3_3v = parse_value(cur);
        assert(cur < end);

        // Channel 2: ATX 5V STB, only on 24-pin connectors. For 10-pin
        // connectors, this will be overwritten by channel 6 later.
        sample.atx_stb = parse_value(cur);
        assert(cur < end);

        // Channel 3: ATX 12V
        sample.atx_12v = parse_value(cur);
        assert(cur < end);

        // Channel 4: ATX 5V
        sample.atx_5v = parse_value(cur);
        assert(cur < end);

        // Channel 5: EPS #1
        sample.eps1 = parse_value(cur);
        assert(cur < end);

        // Channel 6: ATX 12V STB, only on 10-pin connectors. Note that the
        // value still must be parsed to advance 'cur'!
        {
            auto s = parse_value(cur);
            if (sample.atx_3_3v < 1.0f) {
                sample.atx_stb = s;
            }
        }
        assert(cur < end);

        // Channel 7: EPS #3
        sample.eps3 = parse_value(cur);
        assert(cur < end);

        // Channel 8: EPS #2
        sample.eps2 = parse_value(cur);
        assert(cur < end);

        // Channel 9: PCIe #3
        sample.pcie_12v3 = parse_value(cur);
        assert(cur < end);

        // Channel 10: PCIe #2
        sample.pcie_12v2 = parse_value(cur);
        assert(cur < end);

        // Channel 11: PEG 3.3V
        sample.peg_3_3v = parse_value(cur);
        assert(cur < end);

        // Channel 12: PEG 12V
        sample.peg_12v = parse_value(cur);
        assert(cur < end);

        // Channel 13: PCIe #1
        sample.pcie_12v1 = parse_value(cur);
        assert(cur == end);

        // Invoke the callback for the sample.
        callback(sample);
    }

    return retval;
}
