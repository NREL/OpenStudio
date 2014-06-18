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

#include <radiance/WindowGroup.hpp>
#include <radiance/ForwardTranslator.hpp>

#include <utilities/geometry/Geometry.hpp>

namespace openstudio{
namespace radiance{

  WindowGroup::WindowGroup(double azimuth, const model::Space& space, const model::ConstructionBase& construction, 
                            const boost::optional<model::ShadingControl>& shadingControl)
    : m_azimuth(azimuth), m_space(space), m_construction(construction), m_shadingControl(shadingControl)
  {
    m_name = makeName();
  }

  bool WindowGroup::operator==(const WindowGroup& other) const
  {
    if (!m_shadingControl && !other.shadingControl()){
      return true;
    }else if (m_shadingControl && !other.shadingControl()){
      return false;
    }else if (!m_shadingControl && other.shadingControl()){
      return false;
    }  
  
    if (m_space.handle() == other.space().handle()){
      if (m_construction.handle() == other.construction().handle()){
        if (m_shadingControl->handle() == other.shadingControl()->handle()){
          if (this->azimuthString() == other.azimuthString()){
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
  
  double WindowGroup::azimuth() const
  {
    return m_azimuth;
  }

  std::string WindowGroup::azimuthString() const
  {
    // DLM: implement binning here
// keep for future feature (offer auto-binning to window groups for highly tessellated facades)
//
//         if (azi >= 352.50 && azi < 7.50)
//         {
//           aperture_heading = "WG00";
//         } else if (azi >= 7.50 && azi < 22.50) {
//           aperture_heading = "WG01";
//         } else if (azi >= 22.50 && azi < 37.50) {
//           aperture_heading = "WG02";
//         } else if (azi >= 37.50 && azi < 52.50) {
//           aperture_heading = "WG03";
//         } else if (azi >= 52.50 && azi < 67.50) {
//           aperture_heading = "WG04";
//         } else if (azi >= 67.50 && azi < 82.50) {
//           aperture_heading = "WG05";
//         } else if (azi >= 82.50 && azi < 97.50) {
//           aperture_heading = "WG06";
//         } else if (azi >= 97.50 && azi < 112.50) {
//           aperture_heading = "WG07";
//         } else if (azi >= 112.50 && azi < 127.50) {
//           aperture_heading = "WG08";
//         } else if (azi >= 127.50 && azi < 142.50) {
//           aperture_heading = "WG09";
//         } else if (azi >= 142.50 && azi < 157.50) {
//           aperture_heading = "WG10";
//         } else if (azi >= 157.51 && azi < 172.50) {
//           aperture_heading = "WG11";
//         } else if (azi >= 172.50 && azi < 187.50) {
//           aperture_heading = "WG12";
//         } else if (azi >= 187.50 && azi < 202.50) {
//           aperture_heading = "WG13";
//         } else if (azi >= 202.50 && azi < 217.50) {
//           aperture_heading = "WG14";
//         } else if (azi >= 217.51 && azi < 232.50) {
//           aperture_heading = "WG15";
//         } else if (azi >= 232.50 && azi < 247.50) {
//           aperture_heading = "WG16";
//         } else if (azi >= 247.50 && azi < 262.50) {
//           aperture_heading = "WG17";
//         } else if (azi >= 262.50 && azi < 277.50) {
//           aperture_heading = "WG18";
//         } else if (azi >= 277.50 && azi < 292.50) {
//           aperture_heading = "WG19";
//         } else if (azi >= 292.50 && azi < 307.50) {
//           aperture_heading = "WG20";
//         } else if (azi >= 207.50 && azi < 322.50) {
//           aperture_heading = "WG21";
//         } else if (azi >= 322.50 && azi < 337.50) {
//           aperture_heading = "WG22";
//         } else {
//           aperture_heading = "WG23";
//         }


    std::string result = "AZ" + formatString(m_azimuth, 4);
    return result;
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

  std::string WindowGroup::windowGroupPoints() const
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

    std::string result;
    if (centroid && outwardNormal){
      result = formatString(centroid->x()) + " " + formatString(centroid->y()) + " " + formatString(centroid->z()) + " " +
               formatString(outwardNormal->x()) + " " + formatString(outwardNormal->y()) + " " + formatString(outwardNormal->z()) + "\n";
    }

    return result;
  }

  std::string WindowGroup::makeName() const
  {
    std::stringstream ss;
    if (m_shadingControl){
      ss << this->azimuthString() << "_" << m_space.name().get() << "_" << m_construction.name().get() << "_" << m_shadingControl->name().get();
    }else{
      ss << "UncontrolledWindows";
    }

    std::string result = cleanName(ss.str());
    return result;
  }

} // radiance
} // openstudio