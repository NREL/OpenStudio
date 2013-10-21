/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#ifndef ISOMODEL_LOCATION_HPP
#define ISOMODEL_LOCATION_HPP

#include <isomodel/WeatherData.hpp>

namespace openstudio {
namespace isomodel {
  
  class Location
  {
  public:
    double terrain() const {return _terrain;}	
    void setTerrain(double value) {_terrain = value;}
    boost::shared_ptr<WeatherData> weather() const {return _weather; }
    void setWeatherData(boost::shared_ptr<WeatherData> value){ _weather = value;}

  private:
    double _terrain;
    boost::shared_ptr<WeatherData> _weather;    
  };

} // isomodel
} // openstudio
#endif // ISOMODEL_LOCATION_HPP

