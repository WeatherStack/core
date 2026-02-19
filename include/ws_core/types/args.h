#pragma once

#include <array>
#include <map>
#include <stdint.h>
#include <ws_core/macros.h>
#include <cstring>

// Cool argument types

struct compare_strings {
    bool operator()(const char* a, const char* b) const {
        return strcmp(a, b) < 0;
    }
};

// Use an X macro (https://en.wikipedia.org/wiki/X_macro) for ease
#define WS_CORE_ARG_TYPE(X) \
    X(FLAG)                 \
    X(INTEGER)              \
    X(STRING)               \
    X(FLOAT)                \
    X(DOUBLE)               \
    X(PATH)                 \
    X(PORT)

namespace WS_Core {
    namespace Args {
        enum Type {
            WS_CORE_ARG_TYPE(TO_ENUM)
        };
        typedef enum Type type;

        /*const char* TypeNames[] = {
            WS_CORE_ARG_TYPE(TO_STR)
        };*/

        struct Arg {
            const char* name;
            const char* desc;

            // This is what it's going to be called
            // within the Args::value map
            const char* internal_name;

            union Default {
                bool flag;
                int integer;
                char* string;
                float flt;
                double dbl;
                //char* path;   Use string
                //int port;     Use integer
            };

            union Default def;

            union Ranges {
                struct Integer {
                    int min;
                    int max;
                };

                struct Float {
                    float min;
                    float max;
                };

                struct Double {
                    double min;
                    double max;
                };

                struct Integer integer;
                struct Float flt;
                struct Double dbl;
            };

            union Ranges ranges;
            bool has_range;

            const char* arg_long;
            const char* arg_short;

            Type arg_type;
        };
        typedef struct Arg Arg;

        struct Value {
            Arg::Default value;
            Type type;
            bool using_default; // Only used when fetching
            bool success; // Only used when not
                          // using defaults
        };
        typedef struct Value Value;

        // Probably shouldn't use a vector here
        extern std::array<Arg, 3> args;
        // Also probably shouldn't use a map here
        extern std::map<const char*, Value, compare_strings> values;

        /// @brief Parse arguments
        /// @param argc Argument count
        /// @param argv Argument values
        /// @return Success or not
        bool parse_args(int argc, char* const argv[]);

        /// @brief Attempts to get the value
        /// @param key The key
        /// @param def The default value as a backup
        /// @return The found (or default) value
        Value get_value(const char* key, Value def);

        /// @brief Attempts to get the value
        /// @param key The key
        /// @warning Only use this if you know for certain
        ///          that there is a vaue
        /// @return The found value
        Value get_value(const char* key);

        /// @brief Sets a value
        /// @param key The key
        /// @param val The value
        /// @param type the type
        void set_value(const char* key, Arg::Default val, Type type);

        void set_value(const char* key, bool flag);
        void set_value(const char* key, int val, bool port);
        void set_value(const char* key, const char* string, bool path);
        void set_value(const char* key, float flt);
        void set_value(const char* key, double dbl);
    }
}