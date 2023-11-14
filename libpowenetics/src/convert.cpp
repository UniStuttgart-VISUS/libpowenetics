// <copyright file="convert.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "convert.h"

#include <cassert>
#include <cstring>
#include <memory>

#include "endian.h"


/*
 * ::from_uint16
 */
void from_uint16(_Out_writes_(2) std::uint8_t *dst,
        _In_ const std::uint16_t src) noexcept {
    assert(dst != nullptr);
    if (powenetics_endian::little == powenetics_endian::system) {
        auto s = reinterpret_cast<const std::uint8_t *>(&src);
        dst[0] = s[1];
        dst[1] = s[0];
    } else {
        ::memcpy(dst, &src, sizeof(src));
    }
}


/*
 * ::from_uint24
 */
void from_uint24(_Out_writes_(3) std::uint8_t *dst,
        _In_ const std::uint32_t src) noexcept {
    assert(dst != nullptr);
    std::uint32_t retval = 0;
    auto s = reinterpret_cast<const std::uint8_t *>(&src);

    if (powenetics_endian::little == powenetics_endian::system) {
        dst[0] = s[2];
        dst[1] = s[1];
        dst[2] = s[0];
    } else {
        dst[0] = s[1];
        dst[1] = s[2];
        dst[2] = s[3];
    }
}


/*
 * ::to_uint16
 */
std::uint16_t to_uint16(_In_reads_(2) const std::uint8_t *src) noexcept {
    assert(src != nullptr);
    std::uint16_t retval;
    if (powenetics_endian::little == powenetics_endian::system) {
        auto dst = reinterpret_cast<std::uint8_t *>(&retval);
        dst[0] = src[1];
        dst[1] = src[0];
    } else {
        retval = *reinterpret_cast<const std::uint16_t *>(src);
    }
    return retval;
}


/*
 * ::to_uint24
 */
std::uint32_t to_uint24(_In_reads_(3) const std::uint8_t *src) noexcept {
    assert(src != nullptr);
    std::uint32_t retval = 0;
    auto dst = reinterpret_cast<std::uint8_t *>(&retval);

    if (powenetics_endian::little == powenetics_endian::system) {
        dst[0] = src[2];
        dst[1] = src[1];
        dst[2] = src[0];
    } else {
        dst[1] = src[0];
        dst[2] = src[1];
        dst[3] = src[2];
    }

    return retval;
}
