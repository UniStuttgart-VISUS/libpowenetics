# libpowenetics
[![Build Status - Azure Pipelines][build-button]][build-link]

[build-button]: https://img.shields.io/github/checks-status/UniStuttgart-VISUS/libpowenetics/master?label=Azure%20Pipelines&logo=Azure%20Pipelines
[build-link]: https://devops.visus.uni-stuttgart.de/tfs/VIS(US)/External%20Pipelines/_build/latest?definitionId=46&branchName=master

A native C/C++ library for the [Powenetics v2 power measurement kit](https://www.cybenetics.com/index.php?option=powenetics). The library is a clean-sheet design, which is, however, based on the source code of the original Powenetics v2 tool, which Cybenetics generously provided to us. In contrast to their tool, this library is not intended for end users, but for application developers who want to include power measurements in their native applications and sychronise these measurements with their code.

## Building the library
The library is self-contained and can be built using CMake on Windows and Linux (Oracle Linux was tested). On Linux, you may install libudev for device discovery, but the library will build and work without it, too.

## Using the library
In order to anything else, you first need to obtain a `powenetics_handle` for the Powenetics v2 power measurement device. There are two ways of doing this. If you know the serial port the device is connected to, you can open the handle directly:
```c++
#include <libpowenetics/powenetics.h>

powenetics_handle handle = NULL;

{
    auto hr = ::powenetics_open(&handle, L"\\\\.\\COM3", nullptr);
    if (FAILED(hr)) { /* Handle the error. */ }
}
```

Note that on Linux, you would use something like:
```c++
#include <libpowenetics/powenetics.h>

powenetics_handle handle = NULL;

{
    auto hr = ::powenetics_open(&handle, "/dev/ttyACM0", nullptr);
    if (FAILED(hr)) { /* Handle the error. */ }
}
```

You can also probe for devices (this feature is experimental) like so:
```c++
#include <vector>
#include <libpowenetics/powenetics.h>

// Determine the required buffer size.
std::size_t cnt = 0;
{
    auto hr = ::powenetics_probe(nullptr, &cnt);
    if (hr == HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)) { /* Handle the error. */ }
}

// Open the handles.
std::vector<powenetics_handle> handles(cnt);
{
    auto hr = ::powenetics_probe(handles.data(), &cnt);
    if (FAILED(hr)) { /* Handle the error. */ }
}
```

Handles need to be closed when no longer used in order to avoid leaking resources:
```c++
if (handle != NULL) {
    ::powenetics_close(handle);
}
```

If you are using C++, we provide the `visus::powenetics::unique_handle` specialisation of `std::unique_ptr`.

In order to obtain data, you first need to implement a callback function to receive samples:
```c++
void on_sample(powenetics_handle src, const powenetics_sample *sample, void *ctx) {
    // Do something with 'sample'.
}
```

You can then use this function to start the sampling:
```c++
{
    auto hr = ::powenetics_start_streaming(handle, on_sample, nullptr);
    if (FAILED(hr)) { /* Handle the error. */ }
}
```

Delivery of data can be stopped using `::powenetics_stop_streaming(handle)`, which will block until all buffered samples have been delivered. Closing the handle will automatically stop streaming. `powenetics_close_handle` will also block until it is safe to discard all resources used for sampling.

## Demo programmes
TODO

## Acknowledgments
This work was partially funded by Deutsche Forschungsgemeinschaft (DFG) as part of [SFB/Transregio 161](https://www.sfbtrr161.de) (project ID 251654672).
