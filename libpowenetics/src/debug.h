// <copyright file="debug.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#if defined(_WIN32)
#include <Windows.h>
#else /* defined(_WIN32) */
#include <cstdio>
#endif /* defined(_WIN32) */


#if (defined(DEBUG) || defined(_DEBUG))

#if defined(_WIN32)
#define _powenetics_debug(msg) ::OutputDebugStringW(L##msg)
#else /* defined(_WIN32) */
#define _powenetics_debug(msg) ::printf(L##msg)

#endif /* defined(_WIN32) */

#else /* (defined(DEBUG) || defined(_DEBUG)) */
#define _powenetics_debug(msg) 
#endif /* (defined(DEBUG) || defined(_DEBUG)) */
