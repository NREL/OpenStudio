/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "WindPressure.hpp"

#include <math.h>

namespace openstudio{
namespace wind{

double pressureModifier(ShelterClass shelter, double H, double Href)
{
  double A0 = 0.6;
  double a = 0.28;
  switch(shelter)
  {
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
  double wPmod = A0*pow(0.1*H,a);
  wPmod *= wPmod;
  return wPmod;
}

} // wind
} // openstudio
