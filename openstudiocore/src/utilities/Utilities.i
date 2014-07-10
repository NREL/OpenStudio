#ifndef UTILITIES_UTILITIES_I
#define UTILITIES_UTILITIES_I

#ifdef SWIGPYTHON
%module openstudioutilities
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>

%import <utilities/UtilitiesCore.i>
%import <utilities/UtilitiesEconomics.i>
%import <utilities/UtilitiesTime.i>
%import <utilities/UtilitiesData.i>
%import <utilities/UtilitiesPlot.i>
%import <utilities/UtilitiesGeometry.i>
%import <utilities/UtilitiesSql.i>
%import <utilities/UtilitiesBCL.i>
%import <utilities/UtilitiesCloud.i>
%import <utilities/UtilitiesUnits.i>
%import <utilities/UtilitiesIdd.i>
%import <utilities/UtilitiesIdf.i>
%import <utilities/UtilitiesFileTypes.i>

#endif // UTILITIES_UTILITIES_I
