#ifndef UTILITIES_TIME_DATE_I
#define UTILITIES_TIME_DATE_I

%{
  #include <utilities/time/Date.hpp>
%}

%ignore greg_weekday;

// ignore these constructors from Date
%ignore openstudio::Date::Date(const ImplType&);

// create an instantiation of the vector class
%template(DateVector) std::vector< openstudio::Date >;

// create an instantiation of the optional class
%template(OptionalDate) boost::optional< openstudio::Date >;

// create an instantiation of the optional class
%template(OptionalDayOfWeek) boost::optional<openstudio::DayOfWeek>;

// create an instantiation of the optional class
%template(OptionalMonthOfYear) boost::optional<openstudio::MonthOfYear>;

// create an instantiation of the optional class
%template(OptionalNthDayOfWeekInMonth) boost::optional<openstudio::NthDayOfWeekInMonth>;

// Ignore the Date ctor that takes the system `tm` struct
%ignore openstudio::Date::Date(tm);

// include the header into the swig interface directly
%include <utilities/time/Date.hpp>

%extend openstudio::Date{

  std::string __str__() const{
    std::ostringstream os;
    os << *self;
    return os.str();
  }

  std::string __hash__() const{
    std::ostringstream os;
    os << *self;
    return os.str();
  }

  int __cmp__(const Date& other) const{
    if (*self < other){
        return -1;
    }else if(*self > other){
        return 1;
    }
    return 0;
  }
};

#endif //UTILITIES_TIME_DATE_I
