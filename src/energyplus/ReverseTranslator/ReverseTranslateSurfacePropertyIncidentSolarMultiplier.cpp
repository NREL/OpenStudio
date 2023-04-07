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

#include "../ReverseTranslator.hpp"

#include "../../model/SurfacePropertyIncidentSolarMultiplier.hpp"

#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/SurfaceProperty_IncidentSolarMultiplier_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSurfacePropertyIncidentSolarMultiplier(const WorkspaceObject& workspaceObject) {

    boost::optional<SubSurface> subSurface_;

    // Surface Name: Required Object
    if (auto wo_ = workspaceObject.getTarget(SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName)) {
      if (auto mo_ = translateAndMapWorkspaceObject(wo_.get())) {

        if (auto ss_ = mo_->optionalCast<SubSurface>()) {
          subSurface_ = ss_;
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Surface Name', cannot reverse translate.");
          return boost::none;
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Surface Name'");
        return boost::none;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Surface Name'");
      return boost::none;
    }

    openstudio::model::SurfacePropertyIncidentSolarMultiplier modelObject(subSurface_.get());

    // Incident Solar Multiplier: Optional Double
    if (boost::optional<double> _incidentSolarMultiplier =
          workspaceObject.getDouble(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplier)) {
      modelObject.setIncidentSolarMultiplier(_incidentSolarMultiplier.get());
    }

    // Incident Solar Multiplier Schedule Name: Optional Object
    if (auto wo_ = workspaceObject.getTarget(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName)) {
      if (auto mo_ = translateAndMapWorkspaceObject(wo_.get())) {
        if (boost::optional<Schedule> _incidentSolarMultiplierSchedule = mo_->optionalCast<Schedule>()) {
          modelObject.setIncidentSolarMultiplierSchedule(_incidentSolarMultiplierSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Incident Solar Multiplier Schedule Name'");
        }
      }
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
