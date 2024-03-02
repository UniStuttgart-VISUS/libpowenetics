// <copyright file="types.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <inttypes.h>
#include <stddef.h>

#if defined(_WIN32)
#include <sal.h>
#include <Windows.h>
#endif /* defined(_WIN32) */

#if !defined(_WIN32)
#include "libpowenetics/linuxhresult.h"
#endif /* !defined(_WIN32) */


/// <summary>
/// The character type uses in strings in the API.
/// </summary>
#if defined(_WIN32)
typedef wchar_t powenetics_char;
#else /* defined(_WIN32) */
typedef char powenetics_char;
#endif /* defined(_WIN32) */


// Enables us to treat our enumerations as enum classes in C++ and as "normal"
// enums in C.
#if defined(__cplusplus)
#define LIBPOWENETICS_ENUM class
#define LIBPOWENETICS_ENUM_SCOPE(scope, field) field
#else /* defined(__cplusplus) */
#define LIBPOWENETICS_ENUM
#define LIBPOWENETICS_ENUM_SCOPE(scope, field) scope##_##field
#endif /* defined(__cplusplus) */


/// <summary>
/// The opaque type used to represent a Powenetics v2 power measurement device.
/// </summary>
/// <remarks>
/// This is a forward declaration of the internal type representing the
/// connection to a device. Callers must not make any assumptions about the
/// internal memory layout of this type.
/// </remarks>
struct powenetics_device;


/// <summary>
/// The handle to a Powenetics v2 power measurement device.
/// </summary>
/// <remarks>
/// <c>nullptr</c> is used to represent an invalid handle.
/// </remarks>
typedef struct powenetics_device *powenetics_handle;


/// <summary>
/// Defines the quantities that can be measured using the Powenetics v2 power
/// measurement device.
/// </summary>
typedef enum LIBPOWENETICS_ENUM powenetics_quantity_t {

    /// <summary>
    /// Identifies current.
    /// </summary>
    current = 1,

    /// <summary>
    /// Identifies voltage.
    /// </summary>
    voltage = 2
} powenetics_quantity;


struct powenetics_sample_t;

/// <summary>
/// The callback to be invoked when a new sample arrives.
/// </summary>
typedef void (*powenetics_data_callback)(_In_ powenetics_handle source,
    _In_ const struct powenetics_sample_t *sample, _In_opt_ void *context);
