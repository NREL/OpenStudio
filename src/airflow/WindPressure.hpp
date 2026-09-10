/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef AIRFLOW_WINDPRESSURE_HPP
#define AIRFLOW_WINDPRESSURE_HPP

#if _WIN32 || _MSC_VER
#  define WIND_API __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_airflow_EXPORTS) || defined(openstudio_EXPORTS)
#    define WIND_API __attribute__((visibility("default")))
#  else
#    define WIND_API
#  endif
#else
#  define WIND_API
#endif

#include <string>

namespace openstudio {
namespace wind {

  // These classes are a mixture of definitions from the CONTAM docs and old
  // Fundamentals handbooks - references are needed
  enum ShelterClass
  {
    Default,
    Urban,
    Suburban,
    Airport,
    LargeObstruction15ft,
    LargeObstruction40ft,
    LargeObstruction100ft,
    FlatTerrain
  };

  double WIND_API pressureModifier(ShelterClass shelter, double H, double Href = 10.0);

}  // namespace wind
}  // namespace openstudio

#endif  // AIRFLOW_WINDPRESSURE_HPP
