// <copyright file="thread_name.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2022 - 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#if defined(_WIN32)
#include <Windows.h>
#else /* defined(_WIN32) */
#include <pthread.h>

#include "libpowenetics/linuxsal.h"
#endif /* defined(_WIN32) */


/// <summary>
/// The platform-native type representing the ID of a thread.
/// </summary>
#if defined(_WIN32)
typedef DWORD native_thread_id;
#else /* defined (_WIN32) */
typedef pthread_t native_thread_id;
#endif /* defined (_WIN32) */

/// <summary>
/// Apply the given debug name to the thread with the specified ID.
/// </summary>
/// <param name="thread_id"></param>
/// <param name="thread_name"></param>
void set_thread_name(_In_ const native_thread_id thread_id,
    _In_z_ const char* thread_name);

/// <summary>
/// Set the debug name of the calling thread.
/// </summary>
/// <param name="thread_name"></param>
void set_thread_name(_In_z_ const char* thread_name);
