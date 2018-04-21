#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "tinyformat.h"

#ifdef __cplusplus
extern "C" {
#endif

//void SeparatedByCommas(std::stringstream & str)
//{
//
//
//}
    enum LogFlags
    {
        NONE = 0,
        DB = (1 << 0),
        CFG = (1 << 1),
        eLog_KERNEL = (1 << 2),
        ALL = ~(int)0,
    };

    int LogPrintStr(std::string  str);

#define LogPrint(category, ...) do { \
    std::string logMsg; \
    logMsg = tfm::format(__VA_ARGS__); \
    LogPrintStr(logMsg); \
    } while(0)


#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */

