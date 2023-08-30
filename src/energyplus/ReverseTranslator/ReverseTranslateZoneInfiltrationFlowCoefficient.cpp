/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_FlowCoefficient_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneInfiltrationFlowCoefficient(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneInfiltration_FlowCoefficient) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " is not IddObjectType: SpaceInfiltration_FlowCoefficient");
      return boost::none;
    }

    OptionalWorkspaceObject zoneTarget = workspaceObject.getTarget(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName);
    if (!zoneTarget) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " has no Zone Name assigned, not ReverseTranslating it.");
      return boost::none;
    }
    OptionalModelObject zoneModelObject = translateAndMapWorkspaceObject(*zoneTarget);
    if (!zoneModelObject) {
      LOG(Error, "For WorkspaceObject " << workspaceObject.briefDescription()
                                        << ", failed to Reverse Translate the ZoneName object, so not ReverseTranslating it.");
      return boost::none;
    }

    // create the instance
    SpaceInfiltrationFlowCoefficient spaceInfiltrationFlowCoefficient(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      spaceInfiltrationFlowCoefficient.setName(*s);
    }

    if (zoneModelObject->optionalCast<Space>()) {
      spaceInfiltrationFlowCoefficient.setSpace(zoneModelObject->cast<Space>());
    } else if (zoneModelObject->optionalCast<SpaceType>()) {
      spaceInfiltrationFlowCoefficient.setSpaceType(zoneModelObject->cast<SpaceType>());
    }

    // This is a required field technically, but it's ok if if it's missing here since not required in model API.
    if (auto target = workspaceObject.getTarget(ZoneInfiltration_FlowCoefficientFields::ScheduleName)) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          spaceInfiltrationFlowCoefficient.setSchedule(schedule);
        }
      }
    }

    boost::optional<double> value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient);
    if (value) {
      spaceInfiltrationFlowCoefficient.setFlowCoefficient(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient);
    if (value) {
      spaceInfiltrationFlowCoefficient.setStackCoefficient(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent);
    if (value) {
      spaceInfiltrationFlowCoefficient.setPressureExponent(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient);
    if (value) {
      spaceInfiltrationFlowCoefficient.setWindCoefficient(value.get());
    }

    value = workspaceObject.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor);
    if (value) {
      spaceInfiltrationFlowCoefficient.setShelterFactor(value.get());
    }

    return spaceInfiltrationFlowCoefficient;
  }

}  // namespace energyplus

}  // namespace openstudio
