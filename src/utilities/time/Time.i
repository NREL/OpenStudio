#ifndef UTILITIES_TIME_TIME_I
#define UTILITIES_TIME_TIME_I

%{
#include <utilities/time/Time.hpp>
%}

// Ignore the Time ctor that takes the system `tm` struct
%ignore openstudio::Time::Time(tm);

%include <utilities/time/TimeImpl.i>
%include <utilities/time/Date.i>
%include <utilities/time/DateTime.i>
%include <utilities/time/Calendar.i>

#endif //UTILITIES_TIME_TIME_I
