#ifndef UTIL_TIME_H
#define UTIL_TIME_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

    int64_t GetTimeMillis();
    int64_t GetTimeMicros();
    std::string DataTimeStrFormat(const char *cpszFormat, int64_t n64Time);


#ifdef __cplusplus
}
#endif

#endif /* UTIL_TIME_H */

