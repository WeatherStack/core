#pragma once

#include <vector>
#include <map>
#include <stdint.h>
#include <ws_core/macros.h>

// Cool argument types

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

        const char* TypeNames[] = {
            WS_CORE_ARG_TYPE(TO_STR)
        };

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

        // Probably shouldn't use a vector here
        std::vector<Arg> args;
        // Also probably shouldn't use a map here
        std::map<const char*, Arg::Default> values;

        /// @brief Parse arguments
        /// @param argc Argument count
        /// @param argv Argument values
        /// @return Success or not
        bool parse_args(int argc, char* const argv[]);
    }
}