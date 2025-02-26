// <copyright file="magic_auto_lock.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2021 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <mutex>


// Concatenation helper for generating the variable name.
#define _POWENETICS_CONCAT0(l, r) l##r
#define _POWENETICS_CONCAT(l, r) _POWENETICS_CONCAT0(l, r)


// Andrei Alexandrescu's unique variable generator.
#if defined(__COUNTER__)
#define _POWENETICS_UNIQUE_VARIABLE(p) _POWENETICS_CONCAT(p, __COUNTER__)
#else /* defined(__COUNTER__) */
#define _POWENETICS_UNIQUE_VARIABLE(p) _POWENETICS_CONCAT(p, __LINE__)
#endif /* defined(__COUNTER__) */


// Declares a std::unique_lock for 'l'.
#define MAGIC_AUTO_LOCK(l) \
std::unique_lock<decltype(l)> _POWENETICS_UNIQUE_VARIABLE(_l)(l)
