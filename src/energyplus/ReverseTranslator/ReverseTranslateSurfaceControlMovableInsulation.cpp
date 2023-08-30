/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SurfaceControlMovableInsulation.hpp"
#include "../../model/SurfaceControlMovableInsulation_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/SurfaceControl_MovableInsulation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSurfaceControlMovableInsulation(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SurfaceControl_MovableInsulation) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SurfaceControlMovableInsulation");
      return boost::none;
    }

    // Surface and Material are both required fields and not defaulted in the model Ctor, so make sure we can find them
    // before we instantiate a SCMI ModelObject
    OptionalWorkspaceObject target;

    boost::optional<Surface> surface;
    if ((target = workspaceObject.getTarget(openstudio::SurfaceControl_MovableInsulationFields::SurfaceName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Surface>()) {
          surface = modelObject->cast<Surface>();
        }
      }
    } else {
      LOG(Error, "Could not find surface attached to SurfaceControlMovableInsulation object");
      return boost::none;
    }

    boost::optional<Material> material;
    if ((target = workspaceObject.getTarget(openstudio::SurfaceControl_MovableInsulationFields::MaterialName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Material>()) {
          material = modelObject->cast<Material>();
        }
      }
    } else {
      LOG(Error, "Could not find material for SurfaceControlMovableInsulation object attached to " << surface->briefDescription());
      return boost::none;
    }

    SurfaceControlMovableInsulation surfaceControlMovableInsulation(*surface, *material);

    // Required-fields but defaulted in model Ctor: Log an Info if not found, but continue processing
    if (boost::optional<std::string> insulationType = workspaceObject.getString(openstudio::SurfaceControl_MovableInsulationFields::InsulationType)) {
      surfaceControlMovableInsulation.setInsulationType(*insulationType);
    } else {
      LOG(Info, "Defaulting Insulation Type for SurfaceControlMovableInsulation object attached to " << surface->briefDescription());
    }

    if ((target = workspaceObject.getTarget(openstudio::SurfaceControl_MovableInsulationFields::ScheduleName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<Schedule>()) {
        auto schedule = modelObject->cast<Schedule>();
        surfaceControlMovableInsulation.setSchedule(schedule);
      }
    } else {
      LOG(Info, "Defaulting Schedule Name for SurfaceControlMovableInsulation object attached to " << surface->briefDescription());
    }

    return surfaceControlMovableInsulation;
  }

}  // namespace energyplus

}  // namespace openstudio
