#ifndef UTIL_H
#include "PreDefine.h"

#define UTIL_H
#include "tinyformat.h"
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

extern bool gbPrintDebugToConsole;

enum LogFlags {
    NONE = 0,
    DB = (1 << 0),
    CFG = (1 << 1),
    eLog_KERNEL = (1 << 2),
    ALL = ~(int) 0,
};

INT32 LogPrintStr(std::string str);

#define LogPrint(category, ...) do { \
    std::string logMsg; \
    logMsg = tfm::format(__VA_ARGS__); \
    LogPrintStr(logMsg); \
    } while(0)

#define LogPrintDebug(...) do { \
    if(gbPrintDebugToConsole) { \
    std::string logMsg; \
    logMsg = tfm::format(__VA_ARGS__); \
    LogPrintStr(logMsg); \
    } \
    } while(0)


#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */

