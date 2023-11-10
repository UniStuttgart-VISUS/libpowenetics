// <copyright file="stream_parser_v2.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>


/*
 * stream_parser_v2::push_back
 */
template<class TCallback>
bool stream_parser_v2::push_back(_In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt,
        _In_ TCallback&& callback) {
    assert(data != nullptr);
    auto& delimiter = responses_v2::segment_delimiter;
    const auto end = data + cnt;

    if (this->_buffer.empty()) {
        // If the buffer is empty, we first check the input in-place without
        // buffering it. Whatever is not part of a segment will be pushed to
        // the _buffer in the end and preserved for the next call.
        auto cur = find_delimiter(data, cnt);

        if (cur == nullptr) {
            // If we did not find the delimiter, we need to buffer everything
            // until the next call hopefully provides enough data.
            assert(this->_buffer.empty());
            this->_buffer.reserve(cnt);
            std::copy(data, data + cnt, std::back_inserter(this->_buffer));

        } else {
            // If buffer is empty and we found the separator, it must be at
            // the begin of the data. Otherwise, we have at least some rubbish
            // data at the begin which we need to discard, because we have no
            // way to interpret it without the marker at the start of the
            // segment.

            while (cur != nullptr) {
                assert(cur[0] == delimiter.front());
                assert(cur[1] == delimiter.back());
                auto next = find_delimiter(cur + delimiter.size(), end - cur);

                if (next != nullptr) {
                    // We found a segment between 'cur' and 'next'. We now parse
                    // the data in the segment and if this yields a sample, we
                    // deliver it to the callback.
                    powenetics_sample sample;
                    sample.version = 2;
                    if (parse_segment(sample, cur + delimiter.size(), next)) {
                        callback(sample);
                    }

                } else {
                    // We did not find the end of the segment after 'cur', so
                    // preserve the input until the next call.
                    this->_buffer.reserve(cnt);
                    assert(cur[0] == delimiter.front());
                    assert(cur[1] == delimiter.back());
                    std::copy(cur, end, std::back_inserter(this->_buffer));
                }

                cur = next;
            } /* while (cur != nullptr) */
        } /* if (cur == nullptr) */

        return !this->_buffer.empty();

    } else {
        // If the buffer is not empty, we take it and call the method
        // recursively to parse the concatenated content. This is not the
        // most performant implementation, but it should suffice for now.
        auto buffer = std::move(this->_buffer);
        buffer.reserve(buffer.size() + cnt);
        std::copy(data, end, std::back_inserter(buffer));
        assert(this->_buffer.empty());
        return this->push_back(buffer.data(), buffer.size(),
            std::forward<TCallback>(callback));
    }
}
