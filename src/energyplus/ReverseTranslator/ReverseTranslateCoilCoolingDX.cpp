/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilCoolingDX(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Coil_Cooling_DX) {
      LOG(Error, "WorkspaceObject is not IddObjectType: CoilCoolingDX");
      return boost::none;
    }

    OptionalString s;
    OptionalDouble d;
    OptionalWorkspaceObject target;

    boost::optional<CoilCoolingDXCurveFitPerformance> performanceObject;
    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DXFields::PerformanceObjectName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<CoilCoolingDXCurveFitPerformance>()) {
          performanceObject = modelObject->cast<CoilCoolingDXCurveFitPerformance>();
        }
      }
    }

    openstudio::model::CoilCoolingDX dx(m_model, *performanceObject);

    s = workspaceObject.name();
    if (s) {
      dx.setName(*s);
    }

    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DXFields::AvailabilityScheduleName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optSch = modelObject->optionalCast<Schedule>()) {
          dx.setAvailabilitySchedule(optSch.get());
        }
      }
    }

    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DXFields::CondenserZoneName))) {
      if (auto mo_ = translateAndMapWorkspaceObject(*target)) {
        // Zone is translated, and a Space is returned instead
        if (boost::optional<Space> space_ = mo_->optionalCast<Space>()) {
          if (auto z_ = space_->thermalZone()) {
            dx.setCondenserZone(z_.get());
          }
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Condenser Zone Name'");
        }
      }
    }

    // TODO: eventually support the condenser Inlet/Outlet Nodes and the Storage Tanks

    return dx;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
