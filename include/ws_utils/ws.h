#pragma once

#define VERSION_MAJOR (1)
#define VERSION_MINOR (0)
#define VERSION_PATCH (0)

// Windows likes to be unique
#if defined(_WIN32) || defined(_WIN64)
#define WS_CLI_ARG_LONG_BEGIN  "--"
#define WS_CLI_ARG_SHORT_BEGIN "-"
#else
#define WS_CLI_ARG_LONG_BEGIN  "/"
#define WS_CLI_ARG_SHORT_BEGIN "/"
#endif

#include "args.h"