/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateLights(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Lights) {
      LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription() << " is not IddObjectType: Lights");
      return boost::none;
    }

    // create the definition
    openstudio::model::LightsDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    s = workspaceObject.getString(openstudio::LightsFields::DesignLevelCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("LightingLevel", *s)) {
      d = workspaceObject.getDouble(openstudio::LightsFields::LightingLevel);
      if (d) {
        definition.setLightingLevel(*d);
      } else {
        LOG(Error, "LightingLevel value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::LightsFields::WattsperFloorArea);
      if (d) {
        definition.setWattsperSpaceFloorArea(*d);
      } else {
        LOG(Error, "Watts/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::LightsFields::WattsperPerson);
      if (d) {
        definition.setWattsperPerson(*d);
      } else {
        LOG(Error, "Watts/Person value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::LightsFields::ReturnAirFraction);
    if (d) {
      definition.setReturnAirFraction(*d);
    }

    d = workspaceObject.getDouble(openstudio::LightsFields::FractionRadiant);
    if (d) {
      definition.setFractionRadiant(*d);
    }

    d = workspaceObject.getDouble(openstudio::LightsFields::FractionVisible);
    if (d) {
      definition.setFractionVisible(*d);
    }

    s = workspaceObject.getString(openstudio::LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature);
    if (s) {
      if (istringEqual("Yes", *s)) {
        definition.setReturnAirFractionCalculatedfromPlenumTemperature(true);
      } else {
        definition.setReturnAirFractionCalculatedfromPlenumTemperature(false);
      }
    }

    d = workspaceObject.getDouble(openstudio::LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1);
    if (d) {
      definition.setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(*d);
    }

    d = workspaceObject.getDouble(openstudio::LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2);
    if (d) {
      definition.setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(*d);
    }

    // create the instance
    Lights lights(definition);

    s = workspaceObject.name();
    if (s) {
      lights.setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::LightsFields::ZoneorZoneListorSpaceorSpaceListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          lights.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          lights.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::LightsFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          lights.setSchedule(schedule);
        }
      }
    }

    d = workspaceObject.getDouble(openstudio::LightsFields::FractionReplaceable);
    if (d) {
      lights.setFractionReplaceable(*d);
    }

    s = workspaceObject.getString(openstudio::LightsFields::EndUseSubcategory);
    if (s) {
      lights.setEndUseSubcategory(*s);
    }

    return lights;
  }

}  // namespace energyplus

}  // namespace openstudio
