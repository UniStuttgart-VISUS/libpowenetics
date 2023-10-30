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

#if (defined(DEBUG) || defined(_DEBUG))

#if defined(LIBPOWENETICS_EXPORTS)
#define LIBPOWENETICS_TEST_API __declspec(dllexport)
#else /* defined(LIBPOWENETICS_EXPORTS) */
#define LIBPOWENETICS_TEST_API __declspec(dllimport)
#endif /* defined(LIBPOWENETICS_EXPORTS) */

#else /* (defined(DEBUG) || defined(_DEBUG)) */
#define LIBPOWENETICS_TEST_API
#endif /* (defined(DEBUG) || defined(_DEBUG)) */

#else /* (defined(_MSC_VER) && !defined(LIBPOWENETICS_STATIC)) */

#define LIBPOWENETICS_API
#define LIBPOWENETICS_TEST_API

#endif /* (defined(_MSC_VER) && !defined(LIBPOWENETICS_STATIC)) */
