// <copyright file="convert.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cassert>
#include <cinttypes>


/// <summary>
/// Convert two bytes in network-byte order to <see cref="std::uint16_t />.
/// </summary>
/// <param name="src">A valid pointer to at least two bytes in big-endian order.
/// </param>
/// <returns>The little-endian number represented by the first two bytes in
/// <paramref name="src" />.</returns>
inline std::uint16_t to_uint16(_In_reads_(2) const std::uint8_t *src) noexcept {
    assert(src != nullptr);
    std::uint16_t retval;
    auto dst = reinterpret_cast<std::uint8_t *>(&retval);
    dst[0] = src[1];
    dst[1] = src[0];
    return retval;
}


/// <summary>
/// Convert three bytes in network-byte order to
/// <see cref="std::uint32_t" />.
/// </summary>
/// <param name="src">A valid pointer to at least three bytes in big-endian
/// order.</param>
/// <returns>The little-endian number represented by the first three bytes in
/// <paramref name="src" />.</returns>
inline std::uint32_t to_uint24(_In_reads_(3) const std::uint8_t *src) noexcept {
    assert(src != nullptr);
    std::uint32_t retval = 0;
    auto dst = reinterpret_cast<std::uint8_t *>(&retval);
    dst[0] = src[2];
    dst[1] = src[1];
    dst[2] = src[0];
    return retval;
}
