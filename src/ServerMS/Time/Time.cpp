#include "ServerMS/Time/Time.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Time {

    TIME_MILSEC GetTimeSec() {
        TIME_MILSEC now = (boost::posix_time::microsec_clock::universal_time() -
                           boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_seconds();
        assert(now > 0);
        return now;
    }

    TIME_MILSEC GetTimeMillis() {
        TIME_MILSEC now = (boost::posix_time::microsec_clock::universal_time() -
                           boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_milliseconds();
        assert(now > 0);
        return now;
    }

    TIME_MILSEC GetTimeMicros() {
        TIME_MILSEC now = (boost::posix_time::microsec_clock::universal_time() -
                           boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_microseconds();
        assert(now > 0);
        return now;
    }

    std::string DataTimeStrFormat(const char *cpszFormat, TIME_MILSEC n64Time) {
        std::locale classic(std::locale::classic());
        std::locale loc(classic, new boost::posix_time::time_facet(cpszFormat));
        std::stringstream ss;
        ss.imbue(loc);
        ss << boost::posix_time::from_time_t(n64Time);
        return ss.str();
    }

}