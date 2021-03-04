#ifndef UTILITIES_TIME_DATETIME_I
#define UTILITIES_TIME_DATETIME_I

%{
  #include <ctime>
  #include <utilities/time/DateTime.hpp>
%}

// create an instantiation of the container classes
%template(DateTimeVector) std::vector< openstudio::DateTime >;
%template(DateTimePair) std::pair<openstudio::DateTime, openstudio::DateTime>;
%template(OptionalDateTimePair) boost::optional<std::pair<openstudio::DateTime, openstudio::DateTime> >;
%template(OptionalDateTime) boost::optional<openstudio::DateTime>;

// Ignore streaming operations
%ignore operator<<(std::ostream&, const openstudio::DateTime&);

// Ignore the DateTime ctor that takes the system `tm` struct
%ignore openstudio::DateTime::DateTime(tm);

// The easiest to make swig understand std::time_t is this...
typedef long long time_t;

// include the header into the swig interface directly
%include <utilities/time/DateTime.hpp>

%extend openstudio::DateTime{

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

  int __cmp__(const DateTime& other) const{
    if (*self < other){
        return -1;
    }else if(*self > other){
        return 1;
    }
    return 0;
  }



};

#endif //UTILITIES_TIME_DATETIME_I
