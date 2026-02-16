#pragma once

#include <stdint.h>
#include <ws_core/macros.h>

/*
    Every device has their own identifier.
    Again, like the sensors, this is mainly for configuration reasons.

    It contains a list of identifiers for the sensors on the device.
*/

/// @brief WeatherStack core device structure
struct WS_Core_Device {
    uint64_t            identifier;

    char*               name;
    char*               description;

    uint64_t*           sensors;
    int                 sensors_count;
};
typedef struct WS_Core_Device WS_Core_Device;