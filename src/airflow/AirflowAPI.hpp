/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
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
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_airflow_EXPORTS) || defined(openstudio_EXPORTS)
#    define AIRFLOW_API __attribute__((visibility("default")))
#  else
#    define AIRFLOW_API
#  endif
#else
#  define AIRFLOW_API
#endif

#endif
