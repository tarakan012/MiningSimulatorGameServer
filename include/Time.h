#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#include "PreDefine.h"
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
TIME_MILSEC GetTimeSec();
TIME_MILSEC GetTimeMillis();
TIME_MILSEC GetTimeMicros();
std::string DataTimeStrFormat(const char *cpszFormat, TIME_MILSEC n64Time);


#ifdef __cplusplus
}
#endif

#endif /* UTIL_TIME_H */

