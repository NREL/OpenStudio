/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WindowGroup.hpp"
#include "ForwardTranslator.hpp"

#include "../model/ShadingControl.hpp"
#include "../model/Construction.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/ShadingMaterial.hpp"
#include "../model/ShadingMaterial_Impl.hpp"
#include "../model/Blind.hpp"
#include "../model/Blind_Impl.hpp"
#include "../model/DaylightRedirectionDevice.hpp"
#include "../model/DaylightRedirectionDevice_Impl.hpp"
#include "../model/Screen.hpp"
#include "../model/Screen_Impl.hpp"
#include "../model/Shade.hpp"
#include "../model/Shade_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../utilities/geometry/Geometry.hpp"

namespace openstudio {
namespace radiance {

  WindowGroup::WindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space, const model::ConstructionBase& construction,
                           const boost::optional<model::ShadingControl>& shadingControl)
    : m_outwardNormal(outwardNormal), m_space(space), m_construction(construction), m_shadingControl(shadingControl) {
    // start with a gross name, forward translator is in charge of nice names
    m_name = "WG" + toString(createUUID());
    m_outwardNormal.normalize();
  }

  bool WindowGroup::operator==(const WindowGroup& other) const {
    if (!m_shadingControl && !other.shadingControl()) {

      if (m_space.handle() == other.space().handle()) {
        if (m_construction.handle() == other.construction().handle()) {
          double angle = std::abs(radToDeg(getAngle(m_outwardNormal, other.outwardNormal())));
          const double tol = 1.0;
          if (angle < tol) {
            return true;
          }
        }
      }
      return false;

    } else if (m_shadingControl && !other.shadingControl()) {
      return false;
    } else if (!m_shadingControl && other.shadingControl()) {
      return false;
    }

    if (m_space.handle() == other.space().handle()) {
      if (m_construction.handle() == other.construction().handle()) {
        if (m_shadingControl->handle() == other.shadingControl()->handle()) {
          double angle = std::abs(radToDeg(getAngle(m_outwardNormal, other.outwardNormal())));
          const double tol = 1.0;
          if (angle < tol) {
            return true;
          }
        }
      }
    }
    return false;
  }

  std::string WindowGroup::name() const {
    return m_name;
  }

  void WindowGroup::setName(const std::string& name) {
    m_name = name;
  }

  openstudio::Vector3d WindowGroup::outwardNormal() const {
    return m_outwardNormal;
  }

  model::Space WindowGroup::space() const {
    return m_space;
  }

  model::ConstructionBase WindowGroup::construction() const {
    return m_construction;
  }

  boost::optional<model::ShadingControl> WindowGroup::shadingControl() const {
    return m_shadingControl;
  }

  std::string WindowGroup::interiorShadeBSDF() const {
    std::string result = "air.xml";
    if (m_shadingControl) {

      std::string shadingType = m_shadingControl->shadingType();
      if (istringEqual("InteriorShade", shadingType)) {
        result = "05_shadecloth_light.xml";
      } else if (istringEqual("ExteriorShade", shadingType)) {
        // shouldn't get here but use air if we do
      } else if (istringEqual("ExteriorScreen", shadingType)) {
        // shouldn't get here but use air if we do
      } else if (istringEqual("InteriorBlind", shadingType)) {
        result = "blinds.xml";
      } else if (istringEqual("ExteriorBlind", shadingType)) {
        // shouldn't get here but use air if we do
      } else if (istringEqual("BetweenGlassShade", shadingType)) {
        // shouldn't get here but use air if we do
      } else if (istringEqual("BetweenGlassBlind", shadingType)) {
        // shouldn't get here but use air if we do
      } else if (istringEqual("SwitchableGlazing", shadingType)) {
        // shouldn't get here but use air if we do
      } else if (istringEqual("InteriorDaylightRedirectionDevice", shadingType)) {
        result = "1xliloX.xml";
      } else {
        LOG(Warn, "Unknown shadingType '" << shadingType << "' found for ShadingControl '" << m_shadingControl->name().get() << "'");
      }
    }
    return result;
  }

  std::string WindowGroup::shadingControlType() const {
    std::string result = "AlwaysOff";
    if (m_shadingControl) {
      result = m_shadingControl->shadingControlType();

      if (istringEqual("OnIfScheduleAllows", result)) {
        result = "OnIfHighSolarOnWindow";
        LOG(Warn, "ShadingControlType 'OnIfHighSolarOnWindow' is not currently supported for ShadingControl '"
                    << m_shadingControl->name().get() << "', using 'OnIfHighSolarOnWindow' instead.");
      }
    }

    return result;
  }

  std::string WindowGroup::shadingControlSetpoint() const {
    std::string result = "n/a";
    if (m_shadingControl) {
      std::string shadingControlType = this->shadingControlType();

      if (istringEqual("AlwaysOff", shadingControlType)) {
        result = "n/a";
      } else if (istringEqual("AlwaysOn", shadingControlType)) {
        result = "n/a";
      } else if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)) {
        boost::optional<double> d = m_shadingControl->setpoint();
        if (d) {
          std::stringstream ss;
          ss << *d;
          result = ss.str();
        } else {
          // setting default in lux
          result = "5000";
        }
      } else if (istringEqual("OnIfScheduleAllows", shadingControlType)) {
        boost::optional<openstudio::model::Schedule> schedule = m_shadingControl->schedule();
        if (schedule) {
          result = schedule->name().get();
        } else {
          result = "";
        }
      } else {
        result = "n/a";
        LOG(Warn, "Unknown shadingControlType '" << shadingControlType << "' for ShadingControl '" << m_shadingControl->name().get() << "'.");
      }
    }

    return result;
  }

  void WindowGroup::addWindowPolygon(const openstudio::Point3dVector& windowPolygon) {
    m_windowPolygons.push_back(windowPolygon);
  }

  WindowGroupControl WindowGroup::windowGroupControl() const {
    boost::optional<double> largestArea;
    boost::optional<Point3d> centroid;
    boost::optional<Vector3d> outwardNormal;

    for (const auto& windowPolygon : m_windowPolygons) {
      boost::optional<double> area = getArea(windowPolygon);
      if (area) {
        if (!largestArea || (*area > *largestArea)) {
          boost::optional<Point3d> tmpCentroid = getCentroid(windowPolygon);
          boost::optional<Vector3d> tmpOutwardNormal = getOutwardNormal(windowPolygon);
          if (tmpCentroid && tmpOutwardNormal) {
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

  std::string WindowGroup::windowGroupPoints() const {
    WindowGroupControl control = this->windowGroupControl();

    std::string result;
    if (control.centroid && control.outwardNormal) {

      double offset = 0.05;
      Vector3d vec = control.outwardNormal.get();
      vec.setLength(offset);
      Point3d wcPoint = control.centroid.get() + vec;

      result = formatString(wcPoint.x()) + " " + formatString(wcPoint.y()) + " " + formatString(wcPoint.z()) + " "
               + formatString(control.outwardNormal->x()) + " " + formatString(control.outwardNormal->y()) + " "
               + formatString(control.outwardNormal->z()) + "\n";
    }

    return result;
  }

}  // namespace radiance
}  // namespace openstudio
