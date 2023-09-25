/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_WINDOWGROUP_HPP
#define RADIANCE_WINDOWGROUP_HPP

#include "RadianceAPI.hpp"

#include "../model/Space.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ShadingControl.hpp"

#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace radiance {

  struct RADIANCE_API WindowGroupControl
  {
    boost::optional<double> largestArea;
    boost::optional<openstudio::Point3d> centroid;
    boost::optional<openstudio::Vector3d> outwardNormal;
  };

  /** A WindowGroup represents a group of windows which are simulated together in the single or three phase method.
  */
  class RADIANCE_API WindowGroup
  {
   public:
    WindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space, const model::ConstructionBase& construction,
                const boost::optional<model::ShadingControl>& shadingControl);

    bool operator==(const WindowGroup& other) const;

    std::string name() const;

    void setName(const std::string& name);

    openstudio::Vector3d outwardNormal() const;

    model::Space space() const;

    model::ConstructionBase construction() const;

    boost::optional<model::ShadingControl> shadingControl() const;

    std::string interiorShadeBSDF() const;

    std::string shadingControlType() const;

    // returns numeric value if it exists, returns schedule name for scheduled control, n/a otherwise
    std::string shadingControlSetpoint() const;

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

    REGISTER_LOGGER("openstudio.radiance.ForwardTranslator");
  };

  // vector of WindowGroup
  using WindowGroupVector = std::vector<WindowGroup>;

}  // namespace radiance
}  // namespace openstudio

#endif  //RADIANCE_LIGHTFIXTURE_HPP
