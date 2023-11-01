// <copyright file="probe.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cinttypes>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "libpowenetics/api.h"
#include "libpowenetics/types.h"

#include "device.h"
#include "magic_auto_lock.h"


/// <summary>
/// This class allows for probing for powenetics devices on the system.
/// </summary>
class LIBPOWENETICS_TEST_API powenetics_probe final {

public:

    typedef std::unique_ptr<powenetics_device> device_type;
    typedef std::basic_string<powenetics_char> string_type;

    /// <summary>
    /// Gets the paths of all COM ports on the system.
    /// </summary>
    /// <returns>The paths of all COM ports on the system.</returns>
    static std::vector<string_type> candidates(void);

    /// <summary>
    /// Clears all cached devices.
    /// </summary>
    inline void clear(void) {
        MAGIC_AUTO_LOCK(this->_lock);
        this->_devices.clear();
    }

private:

    std::mutex _lock;
    std::vector<device_type> _devices;
};
