#ifndef WS_LOGGER
#define WS_LOGGER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

/// @todo This needs finishing

#if defined(_WIN32) || defined(_WIN64)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define WS_LOGGER_LOG_LEVEL(x) \
    x(TRACE)                   \
    x(DEBUG)                   \
    x(INFO)                    \
    x(WARN)                    \
    x(ERROR)                   \
    x(FATAL)                   \

#define WS_LOGGER_TO_ENUM(x) x,
#define WS_LOGGER_TO_STRS(x) #x,

enum WS_Logger_Log_Level {
    WS_LOGGER_LOG_LEVEL(WS_LOGGER_TO_ENUM)
};
typedef enum WS_Logger_Log_Level WS_Logger_Log_Level;

extern const char* WS_Logger_Log_Level_Strings[];

// Don't use this directly
/// @todo Implement this
void WS_Logger_Log(WS_Logger_Log_Level level, const char* file, int line, const char* fmt, ...);

// Use these
#define LOG_TRACE(fmt, ...) WS_Logger_Log(TRACE, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) WS_Logger_Log(DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  WS_Logger_Log(INFO,  __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  WS_Logger_Log(WARN,  __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) WS_Logger_Log(ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) WS_Logger_Log(FATAL, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif