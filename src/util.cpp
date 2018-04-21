#include <stdint.h>

#include "util.h"
#include "stdio.h"
#include "util_time.h"

bool bPrintToConsole = true;

int LogPrintStr(std::string str) {
    std::string strStamped;
    int64_t nTimeMillis = GetTimeMicros();
    strStamped = DataTimeStrFormat("[%Y-%m-%d %H:%M:%S]", nTimeMillis / 1000000);
    strStamped += ' ' + str;
    int ret = fwrite(strStamped.c_str(), 1, strStamped.size(), stdout);
    return ret;
}
