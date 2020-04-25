#ifndef UTILITIES_TIME_CALENDAR_I
#define UTILITIES_TIME_CALENDAR_I

%{
  #include <utilities/time/Calendar.hpp>
%}

// create an instantiation of the vector class
%template(CalendarVector) std::vector< openstudio::Calendar >;

// create an instantiation of the optional class
%template(OptionalCalendar) boost::optional< openstudio::Calendar >;

// include the header into the swig interface directly
%include <utilities/time/Calendar.hpp>

#endif //UTILITIES_TIME_CALENDAR_I

