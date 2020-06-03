#ifndef UTILITIES_CORE_STRING_I
#define UTILITIES_CORE_STRING_I

%{
  #include <utilities/core/String.hpp>
%}

#ifdef SWIGCSHARP
  // Help in overload resolution
  %ignore openstudio::toString(const char*);
  %ignore openstudio::toString(const std::wstring&);
  %ignore openstudio::toString(const wchart_t*);
  // %ignore openstudio::toString(const std::string& s);
#endif

%include <utilities/core/String.hpp>

#endif //UTILITIES_CORE_STRING_I
