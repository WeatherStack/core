#pragma once

/// @brief Argument type
/// @details Only flag is really important here
enum WS_Utils_Arg_Type {
    WS_UTILS_ARG_TYPE_FLAG,
    WS_UTILS_ARG_TYPE_INTEGER,
    WS_UTILS_ARG_TYPE_PATH,
    WS_UTILS_ARG_TYPE_STRING,
    WS_UTILS_ARG_TYPE_RAW
};
typedef enum WS_Utils_Arg_Type WS_Utils_Arg_Type;

struct WS_Utils_Arg {
    const char* name;
    const char* desc;

    const char* def;
    const int def_flag;

    const char* arg_long;
    const char* arg_short;

    WS_Utils_Arg_Type type;
};
typedef struct WS_Utils_Arg WS_Utils_Arg;

struct WS_Utils_Sub_Command {
    const char* name;
    void (*help)(void); // Command-specific help information,
                        // arg information is printed automatically
                        // afterward
    WS_Utils_Arg* args;
    int arg_count;

    const char* command;

    int (*func)(int, char const **);
};
typedef struct WS_Utils_Sub_Command WS_Utils_Sub_Command;