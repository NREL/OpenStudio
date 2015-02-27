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

#ifndef RADIANCE_WINDOWGROUP_HPP
#define RADIANCE_WINDOWGROUP_HPP

#include "RadianceAPI.hpp"

#include "../model/Space.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ShadingControl.hpp"

#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

namespace openstudio{
namespace radiance{

  struct RADIANCE_API WindowGroupControl{
      boost::optional<double> largestArea;
      boost::optional<openstudio::Point3d> centroid;
      boost::optional<openstudio::Vector3d> outwardNormal;
  };

  /** A WindowGroup represents a group of windows which are simulated together in the single or three phase method.
  */ 
  class RADIANCE_API WindowGroup{
    public:
      WindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space, 
                  const model::ConstructionBase& construction,
                  const boost::optional<model::ShadingControl>& shadingControl);

      bool operator==(const WindowGroup& other) const;

      std::string name() const;

      void setName(const std::string& name);

      openstudio::Vector3d outwardNormal() const;

      model::Space space() const;
      
      model::ConstructionBase construction() const;

      boost::optional<model::ShadingControl> shadingControl() const;

      void addWindowPolygon(const openstudio::Point3dVector& windowPolygon);

      WindowGroupControl windowGroupControl() const;

      std::string windowGroupPoints() const;

    private:

      std::string m_name;

      openstudio::Vector3d m_outwardNormal;
      model::Space m_space;
      model::ConstructionBase m_construction;
      boost::optional<model::ShadingControl> m_shadingControl;
      std::vector<openstudio::Point3dVector> m_windowPolygons;
  };

  // vector of WindowGroup
  typedef std::vector<WindowGroup> WindowGroupVector;


} // radiance
} // openstudio

#endif //RADIANCE_LIGHTFIXTURE_HPP
