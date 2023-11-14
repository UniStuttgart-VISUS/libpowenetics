// <copyright file="excel_worker.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include <libpowenetics/powenetics.h>

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
    /// <param name="input">The handle to a Powenetics device that has already
    /// been opened, but is not yet streaming. The object will register its own
    /// callback and starts streaming to it.</param>
    /// <param name="output">The Excel output to write to. Callers must make
    /// sure that this instance lives at least as long as the worker is running.
    /// </param>
    /// <exception cref="wil::ResultException">If the worker could not be
    /// started, which mainly has two reasons: streaming has already been
    /// started on <paramref name="input" /> before or <paramref name="input" />
    /// is <c>nullptr</c>.</exception>
    excel_worker(_Inout_ powenetics_handle&& input,
        _Inout_ excel_output& output);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~excel_worker(void) noexcept;

    /// <summary>
    /// (Re-) Starts streaming.
    /// </summary>
    /// <remarks>
    /// This method may only be called if the worker has been stopped before.
    /// The constructor will call this method as well and start streaming
    /// automatically.
    /// </remarks>
    /// <exception cref="wil::ResultException">If the operation failed, most
    /// likely because the worker was already running.</exception>
    void start(void);

    /// <summary>
    /// Makes the worker stop the Powenetics device and causes the writer thread
    /// to exit.
    /// </summary>
    /// <remarks>
    /// It is safe to assume that the writer thread has exited once the method
    /// returns. The destructor will call this method, too, if streaming has not
    /// been stopped before.
    /// </remarks>
    void stop(void);

private:

    /// <summary>
    /// The callback that can be registered with a
    /// <see cref="powenetics_handle" /> to write its data to the
    /// configured Excel sheet. Note that a pointer to the
    /// <see cref="excel_worker" /> must be passed as the context.
    /// </summary>
    /// <param name="source">The handle to the device we receive the
    /// data from.</param>
    /// <param name="sample">The sample that the device has just produced.
    /// </param>
    /// <param name="context">A pointer to this worker object.</param>
    static void callback(_In_ powenetics_handle source,
        _In_ const struct powenetics_sample_t *sample,
        _In_opt_ void *context);

    /// <summary>
    /// The worker thread that periodically writes the data into the
    /// Excel sheet.
    /// </summary>
    void worker(void);

    wil::unique_event _event;
    visus::powenetics::unique_handle _input;
    std::mutex _lock;
    excel_output& _output;
    std::vector<powenetics_sample> _samples;
    std::thread _thread;

};
