/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ExteriorLights.hpp"
#include "../../model/ExteriorLights_Impl.hpp"
#include "../../model/ExteriorLightsDefinition.hpp"
#include "../../model/ExteriorLightsDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Exterior_Lights_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

  boost::optional<model::ModelObject> ReverseTranslator::translateExteriorLights(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Exterior_Lights) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " is not of IddObjectType::Exterior_Lights.");
      return boost::none;
    }

    model::ExteriorLightsDefinition definition(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
      definition.setName(*s + " Definition");
    }

    if ((d = workspaceObject.getDouble(Exterior_LightsFields::DesignLevel))) {
      definition.setDesignLevel(*d);
    }

    model::OptionalExteriorLights exteriorLights;
    model::OptionalSchedule schedule;

    if (OptionalWorkspaceObject target = workspaceObject.getTarget(Exterior_LightsFields::ScheduleName)) {
      if (model::OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target)) {
        schedule = modelObject->optionalCast<model::Schedule>();
      }
    }

    if (schedule) {
      try {
        exteriorLights = model::ExteriorLights(definition, *schedule);
      } catch (std::exception& e) {
        LOG(Warn, "Could not reverse translate " << workspaceObject.briefDescription() << " in full, because " << e.what() << ".");
      }
    }
    if (!exteriorLights) {
      exteriorLights = model::ExteriorLights(definition);
    }

    OS_ASSERT(exteriorLights);

    if ((s = workspaceObject.name())) {
      exteriorLights->setName(*s);
    }

    if ((s = workspaceObject.getString(Exterior_LightsFields::ControlOption, false, true))) {
      exteriorLights->setControlOption(*s);
    }

    if ((s = workspaceObject.getString(Exterior_LightsFields::EndUseSubcategory, false, true))) {
      exteriorLights->setEndUseSubcategory(*s);
    }

    return *exteriorLights;
  }

}  // namespace energyplus
}  // namespace openstudio
