// <copyright file="convert.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cinttypes>

#include "libpowenetics/api.h"


/// <summary>
/// Converts the given value <paramref name="src" /> into it network-byte order
/// representation.
/// </summary>
/// <param name="dst">A buffer to receive at least two bytes.</param>
/// <param name="src"></param>
void LIBPOWENETICS_TEST_API from_uint16(_Out_writes_(2) std::uint8_t *dst,
    _In_ const std::uint16_t src) noexcept;

/// <summary>
/// Converts the 24-bits of given value <paramref name="src" /> into it
/// network-byte order representation.
/// </summary>
/// <param name="dst">A buffer to receive at least two bytes.</param>
/// <param name="src"></param>
void LIBPOWENETICS_TEST_API from_uint24(_Out_writes_(3) std::uint8_t *dst,
    _In_ const std::uint32_t src) noexcept;

/// <summary>
/// Convert two bytes in network-byte order to <see cref="std::uint16_t />.
/// </summary>
/// <param name="src">A valid pointer to at least two bytes in big-endian order.
/// </param>
/// <returns>The little-endian number represented by the first two bytes in
/// <paramref name="src" />.</returns>
std::uint16_t LIBPOWENETICS_TEST_API to_uint16(
    _In_reads_(2) const std::uint8_t *src) noexcept;

/// <summary>
/// Convert three bytes in network-byte order to
/// <see cref="std::uint32_t" />.
/// </summary>
/// <param name="src">A valid pointer to at least three bytes in big-endian
/// order.</param>
/// <returns>The little-endian number represented by the first three bytes in
/// <paramref name="src" />.</returns>
std::uint32_t LIBPOWENETICS_TEST_API to_uint24(
    _In_reads_(3) const std::uint8_t *src) noexcept;
