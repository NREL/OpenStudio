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

#ifndef CONTAM_WINDPRESSURE_HPP
#define CONTAM_WINDPRESSURE_HPP

#if _WIN32 || _MSC_VER
#define WIND_API __declspec(dllexport)
#else
#define WIND_API
#endif

#include <string>

namespace openstudio{
namespace wind{

// These classes are a mixture of definitions from the CONTAM docs and old 
// Fundamentals handbooks - references are needed
enum ShelterClass { Default, Urban, Suburban, Airport, LargeObstruction15ft, 
LargeObstruction40ft, LargeObstruction100ft, FlatTerrain };

double WIND_API pressureModifier(ShelterClass shelter, double H, double Href=10.0);

} // wind
} // openstudio

#endif // CONTAM_WINDPRESSURE_HPP
