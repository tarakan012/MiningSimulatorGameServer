#include "ServerMS/Log/Logging.h"
#include "ServerMS/Time/Time.h"

namespace Log {

    bool gbPrintDebugToConsole = true;
    std::vector<GLogColor> gvLogColorVec = {COLOR_GREEN, COLOR_DEFAULT, COLOR_RED, COLOR_YELLOW};

    const char *GetAnsiColorCode(GLogColor color) {
        switch (color) {
            case COLOR_RED:
                return "1";
            case COLOR_GREEN:
                return "2";
            case COLOR_YELLOW:
                return "3";
            case COLOR_DEFAULT:
                return "";
        };
        return NULL; // stop warning about return type.
    }


    INT32 LogPrintStr(std::string str, LoggingEnum loglvl) {
        std::string strStamped;
        TIME_MILSEC nTimeMillis = Time::GetTimeMicros();
        strStamped = Time::DataTimeStrFormat("[%Y-%m-%d %H:%M:%S]", nTimeMillis / 1000000);
        strStamped += ' ' + str;
        const GLogColor color = gvLogColorVec[loglvl];
        fprintf(stdout, "\033[0;3%sm", GetAnsiColorCode(color));
        INT32 ret = fwrite(strStamped.c_str(), 1, strStamped.size(), stdout);
        fprintf(stdout, "\033[m");  // Resets the terminal to default.
        return ret;
    }

}
