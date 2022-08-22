#ifndef UTILITIES_XML_XMLVALIDATOR_I
#define UTILITIES_XML_XMLVALIDATOR_I

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>

%{
  #include <utilities/xml/XMLValidator.hpp>
%}

// ignore detail namespace
%ignore openstudio::detail;

%include <utilities/xml/XMLValidator.hpp>

#endif
