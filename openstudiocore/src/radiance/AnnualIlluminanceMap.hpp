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

#ifndef RADIANCE_ANNUALILLUMINANCEMAP_HPP
#define RADIANCE_ANNUALILLUMINANCEMAP_HPP

#include "RadianceAPI.hpp"

#include "../utilities/data/Vector.hpp"
#include "../utilities/data/Matrix.hpp"
#include "../utilities/time/DateTime.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Path.hpp"

namespace openstudio{
namespace radiance{

  /** AnnualIlluminanceMap represents illuminance map for an entire year.
  *   We assume that the output files is from SPOT, with length in meters and illuminance 
  *   values in footcandles.  All illuminance values are converted to lux.
  */ 
  class RADIANCE_API AnnualIlluminanceMap
  {
    private:

      // map of DateTime to Matrix (illuminance map)
      typedef std::map<openstudio::DateTime, openstudio::Matrix> DateTimeIlluminanceMap;

    public:

      /// default constructor
      AnnualIlluminanceMap();

      /// constructor with path
      AnnualIlluminanceMap(const openstudio::path& path);

      /// virtual destructor
      virtual ~AnnualIlluminanceMap () {}

      /// get the dates and times for which illuminance maps are available
      openstudio::DateTimeVector dateTimes() {return m_dateTimes;}
      //openstudio::DateTime::ConstVec dateTimes() const {return openstudio::DateTime::ConstVec(m_dateTimes.begin(), m_dateTimes.end());}

      /// get the x points corresponding to illuminance matrix columns in meters
      const openstudio::Vector& xVector() const {return m_xVector;}

      /// get the y points corresponding to illuminance matrix rows in meters
      const openstudio::Vector& yVector() const {return m_yVector;}

      /// get the illuminance map in lux corresponding to date and time
      const openstudio::Matrix& illuminanceMap(const openstudio::DateTime& dateTime) const;

    private:

      REGISTER_LOGGER("radiance.AnnualIlluminanceMap");

      void init(const openstudio::path& path);

      openstudio::DateTimeVector m_dateTimes;
      openstudio::Vector m_xVector;
      openstudio::Vector m_yVector;
      openstudio::Matrix m_nullIlluminanceMap; // used when there is no data
      DateTimeIlluminanceMap m_dateTimeIlluminanceMap;
  };

} // radiance
} // openstudio

#endif //RADIANCE_ANNUALILLUMINANCEMAP_HPP
