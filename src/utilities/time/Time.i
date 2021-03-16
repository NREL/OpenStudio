#ifndef UTILITIES_TIME_TIME_I
#define UTILITIES_TIME_TIME_I

%{
#include <utilities/time/Time.hpp>
%}

// Ignore the Time ctor that takes the system `tm` struct
%ignore openstudio::Time::Time(tm);
// Ignore boost::posix_time::time_duration
%ignore openstudio::Time::Time(const ImplType&);
%ignore openstudio::Time::ImplType;

%include <utilities/time/TimeImpl.i>
%include <utilities/time/Date.i>
%include <utilities/time/DateTime.i>
%include <utilities/time/Calendar.i>

#endif //UTILITIES_TIME_TIME_I
