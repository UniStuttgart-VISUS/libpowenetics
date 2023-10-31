// <copyright file="types.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <inttypes.h>
#include <stddef.h>

#if defined(_WIN32)
#include <sal.h>
#include <Windows.h>
#endif /* defined(_WIN32) */


/// <summary>
/// The character type uses in strings in the API.
/// </summary>
#if defined(_WIN32)
typedef wchar_t powenetics_char;
#else /* defined(_WIN32) */
typedef char powenetics_char;
#endif /* defined(_WIN32) */


#if !defined(_WIN32)
/// <summary>
/// The type used to report the status of the API.
/// </summary>
enum HRESULT {
    S_OK,
    S_FALSE,
    E_NOTIMPL,
    E_POINTER,
    E_HANDLE,
    E_OUTOFMEMORY,
    WS_E_INVALID_OPERATION
};
#endif /* !defined(_WIN32) */


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
