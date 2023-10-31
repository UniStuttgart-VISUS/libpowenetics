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
