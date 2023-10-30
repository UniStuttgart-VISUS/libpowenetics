// <copyright file="main.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#if defined(_WIN32)
#include <Windows.h>


/// <summary>
/// Entry point of the DLL.
/// </summary>
/// <param name="handle"></param>
/// <param name="reason"></param>
/// <param name="reserved"></param>
/// <returns><c>TRUE</c>, unconditionally.</returns>
BOOL WINAPI DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            ::DisableThreadLibraryCalls(handle);
//            ::hTrrojanDll = hDll;
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
#endif /* defined(_WIN32) */
