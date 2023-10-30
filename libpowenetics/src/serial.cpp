// <copyright file="seria.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#include "libpowenetics/serial.h"


/*
 * ::powenetics_initialise_serial_configuration
 */
HRESULT LIBPOWENETICS_API powenetics_initialise_serial_configuration(
        _In_ powenetics_serial_configuration *config) {
    if (config == nullptr) {
        return E_POINTER;
    }

    switch (config->version) {
        case 1:
            config->baud_rate = 921600;
            config->data_bits = 8;
            config->parity = powenetics_parity::none;
            config->stop_bits = powenetics_stop_bits::one;
            return S_OK;

        default:
            return E_INVALIDARG;
    }
}
