// <copyright file="stream_parser_v2.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <array>
#include <cassert>
#include <cinttypes>
#include <functional>
#include <iterator>
#include <vector>

#include "libpowenetics/api.h"
#include "libpowenetics/sample.h"
#include "libpowenetics/types.h"

#include "convert.h"
#include "debug.h"


/// <summary>
/// Parses the data stream we receive from a Powenetics v2 power measurement
/// device.
/// </summary>
/// <remarks>
/// <para>This class receives the raw bytes from the serial port and handles all
/// packet fragmentation by itself. Every time it has assembled a full packet,
/// it will deliver it to the registered callback.</para>
/// <para>The parser is stateful and can be used for only one stream as it
/// buffers unused input until it is called next.</para>
/// </remarks>
class LIBPOWENETICS_TEST_API stream_parser_v2 final {

public:

    /// <summary>
    /// The type used to represent a single byte.
    /// </summary>
    typedef std::uint8_t byte_type;

    /// <summary>
    /// Splits the given <paramref name="data" /> and potentially a remainder
    /// that could not be processed in the previous call into segments and
    /// delivers these to <paramref name="callback" />.
    /// </summary>
    /// <typeparam name="TCallback">The type of the callback functor to be
    /// invoked, which must accept a single <see cref="powenetics_sample" />
    /// via which the information that have been parsed are returned.
    /// </typeparam>
    /// <param name="data">The data to be parsed, which must be a
    /// non-<c>nullptr</c> pointer to <paramref name="cnt" /> bytes of data
    /// received from the device.</param>
    /// <param name="cnt">The size of <paramref name="data" /> in bytes.</param>
    /// <param name="callback">The callback to be invoked if a full segment
    /// was found.</param>
    /// <returns><c>true</c> if data have been buffered until the next call,
    /// because the input could not be fully tokenised.</returns>
    template<class TCallback> bool tokenise(
        _In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt,
        _In_ TCallback&& callback);

private:

    /// <summary>
    /// The delimiter for segments in the data stream.
    /// </summary>
    static constexpr std::array<byte_type, 2> delimiter { 0xCA, 0xAC };

    /// <summary>
    /// The expected number of bytes in a valid segment.
    /// </summary>
    /// <remarks>
    /// A valid segment comprises a 16-bit sequence number and 13 samples of
    /// 16-bit voltage data and 24-bit current data.
    /// </remarks>
    static constexpr std::size_t segment_length = 67;

    /// <summary>
    /// Finds the first occurrence of <paramref name="delimiter" /> in
    /// <paramref name="data" /> and returns a pointer to the delimiter.
    /// </summary>
    /// <param name="data"></param>
    /// <param name="cnt"></param>
    /// <returns></returns>
    static _Ret_maybenull_ const byte_type *find_delimiter(
        _In_reads_(cnt) const byte_type *data,
        _In_ const std::size_t cnt);

    /// <summary>
    /// Parse <paramref name="data" /> as voltage and current and advance
    /// data past the data that have been parsed.
    /// </summary>
    /// <param name="data">The pointer to the begin of the sample, which must
    /// designate at least 5 bytes.</param>
    /// <param name="discard_threshold">The voltage threshold which must be
    /// exceeded in order to generate a valid current reading. Otherwise,
    /// the current reading will be set to zero.</param>
    /// <returns>The parsed values.</returns>
    static powenetics_voltage_current parse_value(
        _In_reads_(5) _Out_ const byte_type *& data,
        _In_ const float discard_threshold = 1.0f) noexcept;

    /// <summary>
    /// Parses the given segment and if it has the expected size, invoke
    /// the callback.
    /// </summary>
    /// <typeparam name="TCallback">The type of the callback, which must accept
    /// a (reference to a) <see cref="powenetics_sample" />.</typeparam>
    /// <param name="begin">The begin of the sample, excluding the delimitor.
    /// </param>
    /// <param name="end">The end of the sample. This pointer is not valid any
    /// more.</param>
    /// <param name="callback">The callback to be invoked if the given range
    /// of data contains <see cref="segment_length" /> bytes.</param>
    /// <returns><c>true</c> if <paramref name="callback" /> was invoked,
    /// <c>false</c> otherwise.</returns>
    template<class TCallback>
    static bool parse_segment(_In_reads_(end - begin) const byte_type *begin,
        _In_ const byte_type *end, TCallback&& callback);

    std::vector<byte_type> _buffer;
};

#include "stream_parser_v2.inl"
