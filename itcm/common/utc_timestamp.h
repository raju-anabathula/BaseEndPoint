#ifndef _UTILITY_UTCTIMESTAMP_h
#define _UTILITY_UTCTIMESTAMP_h

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <sys/time.h>


namespace utility
{
   std::string utc_timestamp(const std::string &fmt_string);

   std::string format_timestamp(const std::string &fmt_string, const boost::posix_time::ptime current_time = boost::posix_time::microsec_clock::universal_time());
   time_t utc_timestamp_sec();
   unsigned long long getEpochTime();
   std::string getFormattedCurrentLocalTime(const std::string& fmt);
}

#endif
