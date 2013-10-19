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
#ifndef ISOMODEL_WEATHER_DATA_HPP
#define ISOMODEL_WEATHER_DATA_HPP

#include "ISOModelAPI.hpp"
#include <utilities/data/Vector.hpp>
#include <utilities/data/Matrix.hpp>

namespace openstudio {
namespace isomodel {

class ISOMODEL_API WeatherData
{
public:
  WeatherData(void);
  ~WeatherData(void);
  
  /**
   * mean monthly Global Horizontal Radiation (W/m2)
   */
  Vector mEgh(){return _mEgh;}
  void setMEgh(Vector val){_mEgh = val;}
  /**
   * mean monthly dry bulb temp (C)
   */
  Vector mdbt(){return _mdbt;}
  void setMdbt(Vector val){_mdbt = val;}
  /**
   * mean monthly wind speed; (m/s) 
   */
  Vector mwind(){return _mwind;}
  void setMwind(Vector val){_mwind = val;}

  /**
   * mean monthly total solar radiation (W/m2) on a vertical surface for each of the 8 cardinal directions
   */
  Matrix msolar(){return _msolar;}
  /**
   * mean monthly dry bulb temp for each of the 24 hours of the day (C)
   */
  Matrix mhdbt(){return _mhdbt;}
  /**
   * mean monthly Global Horizontal Radiation for each of the 24 hours of the day (W/m2)
   */
  Matrix mhEgh(){return _mhEgh;}
  void setMhEgh(Matrix val){_mhEgh = val;}
  void setMsolar(Matrix val){_msolar = val;}
  void setMhdbt(Matrix val){_mhdbt = val;}
private:
  Matrix _msolar;
  Matrix _mhdbt;
  Matrix _mhEgh;
  Vector _mEgh;
  Vector _mdbt;
  Vector _mwind;
};
}
}

#endif