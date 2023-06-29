/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ISOMODEL_ISOMODELAPI_HPP
#define ISOMODEL_ISOMODELAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_isomodel_EXPORTS) || defined(openstudio_EXPORTS)
#    define ISOMODEL_API __declspec(dllexport)
#  else
#    define ISOMODEL_API __declspec(dllimport)
#  endif
#else
#  define ISOMODEL_API
#endif

#endif
