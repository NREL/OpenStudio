#ifndef UTILITIES_UTILITIESFILETYPES_I
#define UTILITIES_UTILITIESFILETYPES_I

#ifdef SWIGPYTHON
%module openstudioutilitiesfiletypes
#endif

#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesCore.i>
%import <utilities/UtilitiesIdf.i>
%import <utilities/UtilitiesTime.i>
%import <utilities/UtilitiesData.i>
%import <utilities/UtilitiesBCL.i>


%include <utilities/filetypes/Filetypes.i>

#endif // UTILITIES_UTILITIESFILETYPES_I

