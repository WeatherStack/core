#pragma once

#include <stdint.h>
#include <ws_core/macros.h>

/*
    Every single sensor (even if they are on the same board) have their own unique identifier.
    This is mainly for configuration reasons.

    It contains data, such as what data to expect from it, with a human-readable name too.
    This also allows the sensor to be linked back through a chain, so it's eaiser to look up
    what device a sensor belongs to.

    There 2 main categories of sensor types:
    - Weather (WS) sensors
    - Air pollution (PMS) sensors

    Everything in here should be accessible to C, as some CLI utilities may use C rather than
    C++, so anything that isn't to do with core-specific stuff should be C compatible.
*/

// Use an X macro (https://en.wikipedia.org/wiki/X_macro) for ease
#define WS_CORE_SENSOR_TYPE_LIST(X) \
    X(TEMPERATURE)                  \
    X(HUMIDITY)                     \
    X(PRESSURE)                     \
    X(RAIN)                         \
    X(RAIN_PER)                     \
    X(WIND_DIRECTION)               \
    X(WIND_SPEED)                   \
    X(LIGHT)                        \
    X(OXIDISED)                     \
    X(REDUCED)                      \
    X(NH3)                          \
    X(PM1)                          \
    X(PM25)                         \
    X(PM10)

/// @brief Enum for WS_CORE_SENSOR_TYPE_LIST
enum WS_CORE_SENSOR_TYPE {
    WS_CORE_SENSOR_TYPE_LIST(TO_ENUM)
    WS_CORE_SENSOR_TYPE_LIST_END        // Just so it's easier to count
};
typedef enum WS_CORE_SENSOR_TYPE WS_CORE_SENSOR_TYPE;

/// @brief String array for WS_CORE_SENSOR_TYPE_LIST
/*const char* WS_CORE_SENSOR_TYPE_STRINGS[] = {
    WS_CORE_SENSOR_TYPE_LIST(TO_STR)
};*/

/// @brief WeatherStack core sensor structure
struct WS_Core_Sensor {
    uint64_t            identifier;
    uint64_t            owner_identifier; // The device identifier of the board with the sensor

    char*               name;
    char*               description;

    WS_CORE_SENSOR_TYPE sensor_type;
};
typedef struct WS_Core_Sensor WS_Core_Sensor;