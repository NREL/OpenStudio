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

#ifndef RADIANCE_HEADERINFO_HPP
#define RADIANCE_HEADERINFO_HPP

#include "RadianceAPI.hpp"

#include "../utilities/data/Vector.hpp"
#include "../utilities/core/Logger.hpp"


namespace openstudio{
namespace radiance{

  /** HeaderInfo represents the header of a radiance output file from SPOT.
   *   It's primary function is to define the x and y axis of the illuminance map.
   */ 
  class RADIANCE_API HeaderInfo
  {
    public:

      /// header consists of two lines
      HeaderInfo(const std::string& line1, const std::string& line2);

      /// virtual destructor
      virtual ~HeaderInfo () {}

      /// get the x points corresponding to illuminance matrix columns in meters
      const openstudio::Vector& xVector() const {return m_xVector;}

      /// get the y points corresponding to illuminance matrix rows in meters
      const openstudio::Vector& yVector() const {return m_yVector;}

    private:

      REGISTER_LOGGER("radiance.HeaderInfo");

      openstudio::Vector m_xVector;
      openstudio::Vector m_yVector;
  };

} // radiance
} // openstudio

#endif //RADIANCE_HEADERINFO_HPP
