// <copyright file="excel_worker.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "excel_worker.h"

#include <libpowenetics/powenetics.h>


/*
 * excel_worker::excel_worker
 */
excel_worker::excel_worker(_Inout_ powenetics_handle&& input,
        _Inout_ excel_output&& output)
    : _event(::CreateEvent(nullptr, FALSE, FALSE, nullptr)),
        _input(input),
        _output(std::move(output)) {
    input = nullptr;

    // Start streaming data from the given Powenetics device.
    {
        auto hr = powenetics_start_streaming(this->_input,
            excel_worker::callback, this);
        THROW_IF_FAILED(hr);
    }

    // Start the thread writing all the stuff to Excel.
    this->_thread = std::thread(&excel_worker::worker, this);

}


/*
 * excel_worker::~excel_worker
 */
excel_worker::~excel_worker(void) noexcept {
    // Stop the Powenetics device creating samples. This must be done first such
    // that the queue remains empty after what we do next.
    ::powenetics_stop_streaming(this->_input);
    ::powenetics_close(this->_input);

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
    auto running = true;
    std::vector<powenetics_sample> samples;

    while (running) {
        this->_event.wait();

        {
            std::unique_lock<decltype(this->_lock)> l(this->_lock);
            running = !this->_samples.empty();
            std::swap(this->_samples, samples);
            this->_samples.reserve(samples.capacity());
        }

        for (auto& s : samples) {
            this->_output << s;
        }

        samples.clear();
    }
}
