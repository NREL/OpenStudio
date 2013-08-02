/**********************************************************************
 *  Copyright (c) 2013, The Pennsylvania State University.
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
#ifndef WINDPRESSURE_HPP
#define WINDPRESSURE_HPP

#if _WIN32 || _MSC_VER
#define WIND_API __declspec(dllexport)
#else
#define WIND_API
#endif

//#include <QString>
//#include <QVector>
//#include <QFile>

//#include <utilities/data/TimeSeries.hpp>
//#include <utilities/core/Path.hpp>

//#include "ContamAPI.hpp"

#include <string>

namespace openstudio{
  namespace wind{

    enum ShelterClass { Default, Urban, Suburban, Airport, LargeObstruction15ft, LargeObstruction40ft, LargeObstruction100ft, FlatTerrain };

    double WIND_API pressureModifier(ShelterClass shelter, double H, double Href=10.0);
  }
}

#endif // WINDPRESSURE_H
