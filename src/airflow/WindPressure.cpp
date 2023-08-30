/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WindPressure.hpp"

#include <math.h>

namespace openstudio {
namespace wind {

  double pressureModifier(ShelterClass shelter, double H, double /*Href*/) {
    double A0 = 0.6;
    double a = 0.28;
    switch (shelter) {
      case Urban:
        A0 = 0.35;
        a = 0.4;
        break;
      case Airport:
      case FlatTerrain:
        A0 = 1.0;
        a = 0.15;
        break;
      case LargeObstruction15ft:
        A0 = 0.47;
        a = 0.35;
        break;
      case LargeObstruction40ft:
        A0 = 0.67;
        a = 0.25;
        break;
      case LargeObstruction100ft:
        A0 = 0.85;
        a = 0.2;
        break;
      case Suburban:
      case Default:
      default:
        // This is the default, set above
        break;
    }
    double wPmod = A0 * pow(0.1 * H, a);
    wPmod *= wPmod;
    return wPmod;
  }

}  // namespace wind
}  // namespace openstudio
