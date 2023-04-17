/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
