#ifndef UTILITIES_CORE_STRING_I
#define UTILITIES_CORE_STRING_I

%{
  #include <utilities/core/String.hpp>
%}

#ifdef SWIGCSHARP
  // Help in overload resolution
  %ignore openstudio::toString(const char*);
  %ignore openstudio::toString(const std::wstring&);
  %ignore openstudio::toString(const wchar_t*);
  // %ignore openstudio::toString(std::istream&);
  // %ignore openstudio::toString(const std::string&);
#endif

// Ignore for all
%ignore openstudio::toWString(const std::string&);

%include <utilities/core/String.hpp>

#endif //UTILITIES_CORE_STRING_I
