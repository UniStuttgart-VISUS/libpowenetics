// <copyright file="sample.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "libpowenetics/api.h"
#include "libpowenetics/types.h"


/// <summary>
/// Holds the voltage and the current reading from a single sensor.
/// </summary>
/// <remarks>
/// <para>For performance reasons, this structure only holds the minimum data
/// that we receive from the sensors. Callers can obtain tha apparent power by
/// multiplying voltage and current.</para>
/// </remarks>
struct LIBPOWENETICS_API powenetics_voltage_current {
    float voltage;
    float current;
};


/// <summary>
/// A voltage/current sample as produced by the Powenetics v2 power measurement
/// device.
/// </summary>
struct LIBPOWENETICS_API powenetics_sample {

    /// <summary>
    /// The version of the sample, which must be initialised when this
    /// strucuture is passed to any API.
    /// </summary>
    /// <remarks>
    /// <para>For the current version of the API, this value must always be 2
    /// for Powenetics v2.</para>
    /// <para>This field must always remain at the first version of the
    /// strucuture, even if additional fields are added.</para>
    /// </remarks>
    std::uint32_t version;

    /// <summary>
    /// The sequence number of the sample.
    /// </summary>
    /// <remarks>
    /// <para>This must always remain the second field in the structure.</para>
    /// </remarks>
    std::uint16_t sequence_number;

    /// <summary>
    /// The timestamp of the sample.
    /// </summary>
    /// <remarks>
    /// For Powenetics v2, the timestamp is the system time at which the sample
    /// was received from the serial port.
    /// </remarks>
    std::uint64_t timestamp;

    /* Begin of data measured by Powenetics v2. */

    /// <summary>
    /// 12V rail of 10-pin and 24-pin ATX connector.
    /// </summary>
    powenetics_voltage_current atx_12v;

    /// <summary>
    /// 3.3V rail of the 24-pin ATX connector.
    /// </summary>
    powenetics_voltage_current atx_3_3v;

    /// <summary>
    /// 5V rail of the 24-pin ATX connector.
    /// </summary>
    powenetics_voltage_current atx_5v;

    /// <summary>
    /// 5V standby power of the 24-pin ATX connector or 12V standby power for
    /// the 10-pin ATX connector.
    /// </summary>
    powenetics_voltage_current atx_stb;

    /// <summary>
    /// EPS connector #1.
    /// </summary>
    powenetics_voltage_current eps1;

    /// <summary>
    /// EPS connector #2.
    /// </summary>
    powenetics_voltage_current eps2;

    /// <summary>
    /// EPS connector #3.
    /// </summary>
    powenetics_voltage_current eps3;

    /// <summary>
    /// PCIe 6+2 connector #1 or PCIe 12+4 connector #1.
    /// </summary>
    powenetics_voltage_current pcie_12v1;

    /// <summary>
    /// PCIe 6+2 connector #2 or PCIe 12+4 connector #2.
    /// </summary>
    powenetics_voltage_current pcie_12v2;

    /// <summary>
    /// PCIe 6+2 connector #3.
    /// </summary>
    powenetics_voltage_current pcie_12v3;

    /// <summary>
    /// PEG slot 12V rail.
    /// </summary>
    powenetics_voltage_current peg_12v;

    /// <summary>
    /// PEG slot 3.3V rail.
    /// </summary>
    powenetics_voltage_current peg_3_3v;

    /* End of Powenetics v2, future versions must add fields below. */
};
