#ifndef UTILITIES_UTILITIES_I
#define UTILITIES_UTILITIES_I

#ifdef SWIGPYTHON
%module(directors="1") openstudioutilities
#endif


%include <utilities/UtilitiesAPI.hpp>
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>

%import <utilities/UtilitiesCore.i>
%import <utilities/UtilitiesTime.i>
%import <utilities/UtilitiesData.i>
%import <utilities/UtilitiesPlot.i>
%import <utilities/UtilitiesGeometry.i>
%import <utilities/UtilitiesSql.i>
%import <utilities/UtilitiesBCL.i>
%import <utilities/UtilitiesUnits.i>
%import <utilities/UtilitiesIdd.i>
%import <utilities/UtilitiesIdf.i>
%import <utilities/UtilitiesFileTypes.i>
%import <utilities/UtilitiesXML.i>
%import <utilities/UtilitiesAlfalfa.i>

#endif // UTILITIES_UTILITIES_I
