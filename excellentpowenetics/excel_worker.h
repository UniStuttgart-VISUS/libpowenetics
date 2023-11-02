// <copyright file="excel_worker.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include <wil/resource.h>

#include "excel_output.h"


/// <summary>
/// Manages the output of Powenetics data into an excel sheet and most
/// importantly decouples writing the data from the sampler thread.
/// </summary>
class excel_worker final {

public:

    /// <summary>
    /// Initialises a new instance writing to the given
    /// <paramref name="output" />.
    /// </summary>
    /// <param name="input"></param>
    /// <param name="output"></param>
    excel_worker(_Inout_ powenetics_handle&& input,
        _Inout_ excel_output&& output);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~excel_worker(void) noexcept;

private:

    /// <summary>
    /// The callback that can be registered with a
    /// <see cref="powenetics_handle" /> to write its data to the
    /// configured Excel sheet. Note that a pointer to the
    /// <see cref="excel_worker" /> must be passed as the context.
    /// </summary>
    /// <param name="source"></param>
    /// <param name="sample"></param>
    /// <param name="context"></param>
    static void callback(_In_ powenetics_handle source,
        _In_ const struct powenetics_sample_t *sample,
        _In_opt_ void *context);

    /// <summary>
    /// The worker thread that periodically writes the data into the
    /// Excel sheet.
    /// </summary>
    void worker(void);

    wil::unique_event _event;
    powenetics_handle _input;
    std::mutex _lock;
    excel_output _output;
    std::vector<powenetics_sample> _samples;
    std::thread _thread;

};
