/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/GasEquipment.hpp"
#include "../../model/GasEquipment_Impl.hpp"
#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/GasEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateGasEquipment(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::GasEquipment) {
      LOG(Error, "WorkspaceObject is not IddObjectType: GasEquipment");
      return boost::none;
    }

    // create the definition
    openstudio::model::GasEquipmentDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    s = workspaceObject.getString(openstudio::GasEquipmentFields::DesignLevelCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("EquipmentLevel", *s)) {
      d = workspaceObject.getDouble(openstudio::GasEquipmentFields::DesignLevel);
      if (d) {
        definition.setDesignLevel(*d);
      } else {
        LOG(Error, "EquipmentLevel value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::GasEquipmentFields::PowerperFloorArea);
      if (d) {
        definition.setWattsperSpaceFloorArea(*d);
      } else {
        LOG(Error, "Watts/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::GasEquipmentFields::PowerperPerson);
      if (d) {
        definition.setWattsperPerson(*d);
      } else {
        LOG(Error, "Watts/Person value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::GasEquipmentFields::FractionLatent);
    if (d) {
      definition.setFractionLatent(*d);
    }

    d = workspaceObject.getDouble(openstudio::GasEquipmentFields::FractionRadiant);
    if (d) {
      definition.setFractionRadiant(*d);
    }

    d = workspaceObject.getDouble(openstudio::GasEquipmentFields::FractionLost);
    if (d) {
      definition.setFractionLost(*d);
    }

    d = workspaceObject.getDouble(openstudio::GasEquipmentFields::CarbonDioxideGenerationRate);
    if (d) {
      definition.setCarbonDioxideGenerationRate(*d);
    }

    // create the instance
    GasEquipment gasEquipment(definition);

    s = workspaceObject.name();
    if (s) {
      gasEquipment.setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::GasEquipmentFields::ZoneorZoneListorSpaceorSpaceListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          gasEquipment.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          gasEquipment.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::GasEquipmentFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          gasEquipment.setSchedule(schedule);
        }
      }
    }

    s = workspaceObject.getString(openstudio::GasEquipmentFields::EndUseSubcategory);
    if (s) {
      gasEquipment.setEndUseSubcategory(*s);
    }

    return gasEquipment;
  }

}  // namespace energyplus

}  // namespace openstudio
