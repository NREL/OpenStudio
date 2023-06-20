/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef AIRFLOW_AIRFLOWAPI_HPP
#define AIRFLOW_AIRFLOWAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_airflow_EXPORTS) || defined(openstudio_EXPORTS)
#    define AIRFLOW_API __declspec(dllexport)
#  else
#    define AIRFLOW_API __declspec(dllimport)
#  endif
#else
#  define AIRFLOW_API
#endif

#endif
