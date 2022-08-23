#ifndef UTILITIES_UTILITIESXML_I
#define UTILITIES_UTILITIESXML_I

#ifdef SWIGPYTHON
%module openstudioutilitiesxml
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesCore.i>
%import <utilities/UtilitiesUnits.i>

%include <utilities/xml/XMLValidator.i>

#endif
