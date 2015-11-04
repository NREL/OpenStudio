/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef RADIANCE_MATERIALPROPERTIES_HPP
#define RADIANCE_MATERIALPROPERTIES_HPP

#include "RadianceAPI.hpp"

#include <vector>

namespace openstudio{
  namespace radiance{

    /** MaterialProperties represents the color, texture, transmittance, reflectance of opaque and glazing materials.
    */ 
    class RADIANCE_API MaterialProperties{
      public:
    };

    // vector of MaterialProperties
    typedef std::vector<MaterialProperties> LMaterialPropertiesVector;

  } // radiance
} // openstudio

#endif //RADIANCE_MATERIALPROPERTIES_HPP
