#ifndef UTILITIES_TIME_TIMEIMPL_I
#define UTILITIES_TIME_TIMEIMPL_I

%{
  #include <utilities/time/Time.hpp>
%}

// create an instantiation of the vector class
%template(TimeVector) std::vector< openstudio::Time >;

// Optional Time instantiation
%template(OptionalTime) boost::optional< openstudio::Time >;

// include the header into the swig interface directly
%include <utilities/time/Time.hpp>

%extend openstudio::Time{
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

  int __cmp__(const Time& other) const{
    if (*self < other){
        return -1;
    }else if(*self > other){
        return 1;
    }
    return 0;
  }

};

#endif //UTILITIES_TIME_TIMEIMPL_I
