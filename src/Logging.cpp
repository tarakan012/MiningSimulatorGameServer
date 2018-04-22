#include "Logging.h"
#include "Time.h"

bool gbPrintDebugToConsole = true;

INT32 LogPrintStr(std::string str) {
    std::string strStamped;
    TIME_MILSEC nTimeMillis = GetTimeMicros();
    strStamped = DataTimeStrFormat("[%Y-%m-%d %H:%M:%S]", nTimeMillis / 1000000);
    strStamped += ' ' + str;
    INT32 ret = fwrite(strStamped.c_str(), 1, strStamped.size(), stdout);
    return ret;
}
