// <copyright file="timestamp.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "libpowenetics/timestamp.h"

#include <chrono>

#if defined(_WIN32)
#include <Windows.h>
#endif /* defined(_WIN32) */


/// <summary>
/// The 100ns period defining the reolution of <see cref="FILETIME" />.
/// </summary>
typedef std::ratio<1, 10000000> filetime_period;


/// <summary>
/// The STL representation of <see cref="FILETIME" />.
/// </summary>
typedef std::chrono::duration<powenetics_timestamp, filetime_period>
    filetime_duration;


/*
 * ::_powenetics_make_timestamp
 */
powenetics_timestamp _powenetics_make_timestamp(void) noexcept {
    using namespace std::chrono;

    // Make the timestamp.
    const auto timestamp = system_clock::now();

    // The offset of the FILETIME epoch to the UNIX epoch as copied from
    // StackOverflow.
    const filetime_duration dz(116444736000000000LL);

    // Find out what the difference between the time point and the UNIX
    // epoch is. Because we cannot rely on the epoch of the STL clock being the
    // UNIX epoch (until C++ 20), use time_t, which is guaranteed to represent
    // the UNIX epoch.
    auto dt = duration_cast<filetime_duration>(timestamp
        - system_clock::from_time_t(0));

    // Transform the origin of the timestamp clock to the origin of FILETIME.
    return (dt + dz).count();
}


/*
 * ::powenetics_make_timestamp
 */
powenetics_timestamp powenetics_make_timestamp(void){
#if defined(_WIN32)
    FILETIME file_time;
    LARGE_INTEGER retval;

    ::GetSystemTimeAsFileTime(&file_time);
    retval.HighPart = file_time.dwHighDateTime;
    retval.LowPart = file_time.dwLowDateTime;

    return retval.QuadPart;
#else /* defined(_WIN32) */
    return ::_powenetics_make_timestamp();
#endif /* defined(_WIN32) */
}
