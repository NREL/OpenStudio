#ifndef UTILITIES_UTILITIESIDF_I
#define UTILITIES_UTILITIESIDF_I

#ifdef SWIGPYTHON
%module openstudioutilitiesidf
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesCore.i>
%import <utilities/UtilitiesIdd.i>

%include <utilities/idf/Idf.i>

#endif // UTILITIES_UTILITIESIDF_I

