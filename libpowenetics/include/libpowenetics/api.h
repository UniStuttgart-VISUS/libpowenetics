// <copyright file="api.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE file in the project root for detailed information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


#if (defined(_MSC_VER) && !defined(LIBPOWENETICS_STATIC))

#if defined(LIBPOWENETICS_EXPORTS)
#define LIBPOWENETICS_API __declspec(dllexport)
#else /* defined(LIBPOWENETICS_EXPORTS) */
#define LIBPOWENETICS_API __declspec(dllimport)
#endif /* defined(LIBPOWENETICS_EXPORTS)*/

#else /* (defined(_MSC_VER) && !defined(LIBPOWENETICS_STATIC)) */

#define LIBPOWENETICS_API

#endif /* (defined(_MSC_VER) && !defined(LIBPOWENETICS_STATIC)) */


#if defined(LIBPOWENETICS_EXPOSE_TO_TESTING)
#define LIBPOWENETICS_TEST_API LIBPOWENETICS_API
#else /* defined(LIBPOWENETICS_EXPOSE_TO_TESTING) */
#define LIBPOWENETICS_TEST_API
#endif /* defined(LIBPOWENETICS_EXPOSE_TO_TESTING) */


#if !defined(_WIN32)
#include "libpowenetics/linuxsal.h"
#endif /* !defined(_WIN32) */
