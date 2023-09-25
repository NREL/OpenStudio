/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
