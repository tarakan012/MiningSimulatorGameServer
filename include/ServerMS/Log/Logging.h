#ifndef UTIL_H
#define UTIL_H

#include "TinyFormat/tinyformat.h"

namespace Log {

    extern bool gbPrintDebugToConsole;

    enum LogFlags {
        NONE = 0,
        DB = (1 << 0),
        CFG = (1 << 1),
        eLog_KERNEL = (1 << 2),
        ALL = ~(int) 0,
    };

    enum LoggingEnum {
        LOG_INFO,
        LOG_DEBUG,
        LOG_ERROR,
        LOG_WARNNING,
        LOG_SpecialDebug,
        LOG_ASSERT,
        LOG_END
    };

    enum GLogColor {
        COLOR_DEFAULT,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
    };

    INT32 LogPrintStr(std::string str, LoggingEnum loglvl);

#define LogPrint(category, ...) do { \
    std::string logMsg; \
    logMsg = tfm::format(__VA_ARGS__); \
    LogPrintStr(logMsg, LoggingEnum::LOG_INFO); \
    } while(0)

#define LogPrintDebug(...) do { \
    if(gbPrintDebugToConsole) { \
    std::string logMsg; \
    std::string nameFunc; \
    std::string typeLog; \
    nameFunc = tfm::format(__PRETTY_FUNCTION__); \
    logMsg = tfm::format(__VA_ARGS__); \
    std::string result = tfm::format("[DEBUG] [%s] [%s]\n",nameFunc, logMsg); \
    LogPrintStr(result, LoggingEnum::LOG_DEBUG); \
    } \
    } while(0)

#define LogPrintError(...) do { \
    if(gbPrintDebugToConsole) { \
    std::string logMsg; \
    std::string nameFunc; \
    nameFunc = tfm::format(__PRETTY_FUNCTION__); \
    logMsg = tfm::format(__VA_ARGS__); \
    std::string result = tfm::format("[ERROR] [%s] [%s]\n",nameFunc, logMsg); \
    LogPrintStr(result, LoggingEnum::LOG_ERROR); \
    } \
    } while(0)

}

#endif /* UTIL_H */