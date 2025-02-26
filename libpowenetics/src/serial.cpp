// <copyright file="seria.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
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
        case 2:
            config->baud_rate = 921600;
            config->data_bits = 8;
            config->parity = powenetics_parity::none;
            config->stop_bits = powenetics_stop_bits::one;
            return S_OK;

        default:
            return E_INVALIDARG;
    }
}
