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

#include "WindowGroup.hpp"
#include "ForwardTranslator.hpp"

#include "../utilities/geometry/Geometry.hpp"

namespace openstudio{
namespace radiance{

  WindowGroup::WindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space, 
                           const model::ConstructionBase& construction,
                           const boost::optional<model::ShadingControl>& shadingControl)
    : m_outwardNormal(outwardNormal), m_space(space), m_construction(construction), m_shadingControl(shadingControl)
  {
    // start with a gross name, forward translator is in charge of nice names
    m_name = "WG" + toString(createUUID());
    m_outwardNormal.normalize();
  }

 bool WindowGroup::operator==(const WindowGroup& other) const
{
  if (!m_shadingControl && !other.shadingControl()){
  
    if (m_space.handle() == other.space().handle()){
      if (m_construction.handle() == other.construction().handle()){
        double angle = std::abs(radToDeg(getAngle(m_outwardNormal, other.outwardNormal())));
        const double tol = 1.0;
        if (angle < tol){
          return true;
        }
      }
    }
    return false;
  
  }else if (m_shadingControl && !other.shadingControl()){
    return false;
  }else if (!m_shadingControl && other.shadingControl()){
    return false;
  }
  
  if (m_space.handle() == other.space().handle()){
    if (m_construction.handle() == other.construction().handle()){
      if (m_shadingControl->handle() == other.shadingControl()->handle()){
        double angle = std::abs(radToDeg(getAngle(m_outwardNormal, other.outwardNormal())));
        const double tol = 1.0;
        if (angle < tol){
          return true;
        }
      }
    }
  }
  return false;
}

  std::string WindowGroup::name() const
  {
    return m_name;
  }

  void WindowGroup::setName(const std::string& name)
  {
    m_name = name;
  }
  
  openstudio::Vector3d WindowGroup::outwardNormal() const
  {
    return m_outwardNormal;
  }
  
  model::Space WindowGroup::space() const
  {
    return m_space;
  }

  model::ConstructionBase WindowGroup::construction() const
  {
    return m_construction;
  }

  boost::optional<model::ShadingControl> WindowGroup::shadingControl() const
  {
    return m_shadingControl;
  }

  void WindowGroup::addWindowPolygon(const openstudio::Point3dVector& windowPolygon)
  {
    m_windowPolygons.push_back(windowPolygon);
  }

  WindowGroupControl WindowGroup::windowGroupControl() const
  {
    boost::optional<double> largestArea;
    boost::optional<Point3d> centroid;
    boost::optional<Vector3d> outwardNormal;

    for (const auto & windowPolygon : m_windowPolygons){
      boost::optional<double> area = getArea(windowPolygon);
      if (area){
        if (!largestArea || (*area > *largestArea)){
          boost::optional<Point3d> tmpCentroid = getCentroid(windowPolygon);
          boost::optional<Vector3d> tmpOutwardNormal = getOutwardNormal(windowPolygon);
          if (tmpCentroid && tmpOutwardNormal){
            largestArea = area;
            centroid = tmpCentroid;
            outwardNormal = tmpOutwardNormal;
          }
        }
      }
    }

    WindowGroupControl result;
    result.largestArea = largestArea;
    result.centroid = centroid;
    result.outwardNormal = outwardNormal;

    return result;
  }

  std::string WindowGroup::windowGroupPoints() const
  {
    WindowGroupControl control = this->windowGroupControl();

    std::string result;
    if (control.centroid && control.outwardNormal){

      double offset = 0.05;
      Vector3d vec = control.outwardNormal.get();
      vec.setLength(offset);
      Point3d wcPoint = control.centroid.get() + vec;

      result = formatString(wcPoint.x()) + " " + formatString(wcPoint.y()) + " " + formatString(wcPoint.z()) + " " +
        formatString(control.outwardNormal->x()) + " " + formatString(control.outwardNormal->y()) + " " + formatString(control.outwardNormal->z()) + "\n";
    }

    return result;
  }

} // radiance
} // openstudio
