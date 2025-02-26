// <copyright file="excel_worker.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "excel_worker.h"

#include <libpowenetics/powenetics.h>


/*
 * excel_worker::excel_worker
 */
excel_worker::excel_worker(_Inout_ powenetics_handle&& input,
        _Inout_ excel_output& output)
    : _event(::CreateEvent(nullptr, FALSE, FALSE, nullptr)),
        _input(input),
        _output(output) {
    input = nullptr;
    this->start();
}


/*
 * excel_worker::~excel_worker
 */
excel_worker::~excel_worker(void) noexcept {
    this->stop();
}


/*
 * excel_worker::start
 */
void excel_worker::start(void) {
    assert(this->_input != nullptr);

    // Start streaming data from the given Powenetics device.
    {
        auto hr = powenetics_start_streaming(this->_input.get(),
            excel_worker::callback, this);
        THROW_IF_FAILED(hr);
    }

    // Start the thread writing all the stuff to Excel. Note that we must not
    // start the thread if we were unable to start streaming.
    this->_thread = std::thread(&excel_worker::worker, this);
}


/*
 * excel_worker::stop
 */
void excel_worker::stop(void) {
    // Stop the Powenetics device creating samples. This must be done first such
    // that the queue remains empty after what we do next.
    ::powenetics_stop_streaming(this->_input.get());

    // Clear the queue and wake the worker thread. This is the signal for the
    // worker thread to exit. In all other cases, we wake it only when the queue
    // is not empty.
    {
        std::unique_lock<decltype(this->_lock)> l(this->_lock);
        this->_samples.clear();
        this->_event.SetEvent();
    }

    // Wait until the worker thread exited.
    if (this->_thread.joinable()) {
        this->_thread.join();
    }
}


/*
 * excel_worker::callback
 */
void excel_worker::callback(_In_ powenetics_handle source,
        _In_ const struct powenetics_sample_t *sample,
        _In_opt_ void *context) {
    assert(source != nullptr);
    assert(sample != nullptr);
    auto that = static_cast<excel_worker *>(context);
    assert(that != nullptr);
    std::unique_lock<decltype(that->_lock)> l(that->_lock);
    that->_samples.push_back(*sample);
    that->_event.SetEvent();
}


/*
 * excel_worker::worker
 */
void excel_worker::worker(void) {
    auto com_scope = wil::CoInitializeEx(COINIT_MULTITHREADED);
    auto running = true;
    std::vector<powenetics_sample> samples;

    while (running) {
        this->_event.wait();

        // Swap the buffer the device is writing to and the one we are reading
        // from in a critical section as small as possible. After swapping the
        // buffers, make sure that the capacity of the one we are writing to is
        // at least the capacity of the one we just received, this minimising
        // costly reallocations in the sampler thread that receives new data
        // every millisecond.
        {
            std::unique_lock<decltype(this->_lock)> l(this->_lock);
            running = !this->_samples.empty();
            std::swap(this->_samples, samples);
            this->_samples.reserve(samples.capacity());
        }

        // Write all the samples into the excel sheet.
        for (auto& s : samples) {
            this->_output << s;
        }

        // Clear the local buffer such that it is not filled when we swap
        // the next time we wake.
        samples.clear();
    }
}
