// <copyright file="probe.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "probe.h"

#include <system_error>

#if defined(_WIN32)
#include <SetupAPI.h>
#include <Windows.h>
#endif /* defined(_WIN32) */


#if defined(_WIN32)
/// <summary>
/// Gets the value of the specified device property.
/// </summary>
static std::vector<std::uint8_t> get_property(_In_ HDEVINFO dev_info,
        _In_ const SP_DEVINFO_DATA& dev_data,
        _In_ const DWORD property) {
    auto data = const_cast<PSP_DEVINFO_DATA>(&dev_data);
    std::vector<std::uint8_t> retval;
    DWORD size = 0;
    DWORD type = 0;

    // Measure the required buffer size.
    ::SetupDiGetDeviceRegistryPropertyW(dev_info, data, property, &type,
        nullptr, 0, &size);

    retval.resize(size);

    // Get the data.
    if (!::SetupDiGetDeviceRegistryPropertyW(dev_info, data, property, &type,
            retval.data(), static_cast<DWORD>(retval.size()), &size)) {
        throw std::system_error(::GetLastError(), std::system_category());
    }

    return retval;
}
#endif /* defined(_WIN32) */


/*
 * powenetics_probe::candidates
 */
std::vector<powenetics_probe::string_type> powenetics_probe::candidates(void) {
    std::vector<string_type> retval;

#if defined(_WIN32)
    // Windows 10 could use https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcommports
    // but we want the code to be as portable as possible, so we use SetupAPI
    // instead.

    // Create a HDEVINFO with all ports that are present.
    static const GUID ports_class = { 0x4D36E978, 0xE325, 0x11CE,
        { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } };
    auto dev_info = ::SetupDiGetClassDevsW(&ports_class, nullptr, NULL,
        DIGCF_PRESENT);

    // Enumerate through all devices in the set.
    SP_DEVINFO_DATA dev_data;
    ::ZeroMemory(&dev_data, sizeof(dev_data));
    dev_data.cbSize = sizeof(SP_DEVINFO_DATA);


    for (DWORD i = 0; ::SetupDiEnumDeviceInfo(dev_info, i, &dev_data); ++i) {
        DWORD size = 0;
        DWORD type = 0;

        auto path = ::get_property(dev_info, dev_data, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME);
//        auto name = ::get_property(dev_info, dev_data, SPDRP_FRIENDLYNAME);
    }

    if (dev_info != INVALID_HANDLE_VALUE) {
        ::SetupDiDestroyDeviceInfoList(dev_info);
    }
#else /* defined(_WIN32) */
    throw "TODO";
#endif /* defined(_WIN32) */

    return retval;
}
