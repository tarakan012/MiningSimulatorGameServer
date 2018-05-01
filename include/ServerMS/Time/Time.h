#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#include "ServerMS/PreDefine.h"
#include <string>

namespace Time {

    TIME_MILSEC GetTimeSec();

    TIME_MILSEC GetTimeMillis();

    TIME_MILSEC GetTimeMicros();

    std::string DataTimeStrFormat(const char *cpszFormat, TIME_MILSEC n64Time);

#endif /* UTIL_TIME_H */

}