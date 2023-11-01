// <copyright file="probe.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "probe.h"

#include <regex>
#include <system_error>

#if defined(_WIN32)
#include <SetupAPI.h>
#include <Windows.h>
#else /* defined(_WIN32) */
#include <dirent.h>

#include <sys/types.h>
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
    auto dev_info = ::SetupDiGetClassDevsW(&GUID_CLASS_COMPORT, nullptr, NULL,
        DIGCF_PRESENT);

    // Enumerate the interface details, which provide us with the path to the
    // device that we can open using CreateFile.
    SP_DEVICE_INTERFACE_DATA if_data;
    ::ZeroMemory(&if_data, sizeof(if_data));
    if_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for (DWORD i = 0; ::SetupDiEnumDeviceInterfaces(dev_info, nullptr,
            &GUID_DEVINTERFACE_COMPORT, i, &if_data); ++i) {
        DWORD size = 0;
        ::SetupDiGetDeviceInterfaceDetailW(dev_info,
            &if_data,
            nullptr,
            0,
            &size,
            nullptr);

        if (size > 0) {
            std::vector<std::uint8_t> data(size);
            auto detail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(
                data.data());
            detail->cbSize = static_cast<DWORD>(data.size());

            if (::SetupDiGetDeviceInterfaceDetail(dev_info,
                    &if_data,
                    detail,
                    size,
                    &size,
                    nullptr)) {
                retval.emplace_back(detail->DevicePath);
            }
        }
    } /* for (DWORD i = 0; ::SetupDiEnumDeviceInterfaces(dev_info, ... */

#else /* defined(_WIN32) */
    // On Linux, we need to enumerate all tty devices.
    auto dir = ::opendir("/dev");
    dirent *entry;

    if (dir != nullptr) {
        while ((entry = ::readdir(dir)) != nullptr) {
            string_type name(entry->d_name);

            throw "TODO: regex the name";
        }

        ::closedir(dir);
    }
#endif /* defined(_WIN32) */

    return retval;
}
