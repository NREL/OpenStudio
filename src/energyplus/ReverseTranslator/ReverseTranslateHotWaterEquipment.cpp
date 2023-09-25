/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/HotWaterEquipment.hpp"
#include "../../model/HotWaterEquipment_Impl.hpp"
#include "../../model/HotWaterEquipmentDefinition.hpp"
#include "../../model/HotWaterEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/HotWaterEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateHotWaterEquipment(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::HotWaterEquipment) {
      LOG(Error, "WorkspaceObject is not IddObjectType::HotWaterEquipment");
      return boost::none;
    }

    LOG(Info, "HotWaterEquipment is an internal load separate from any plant loop or water system, "
                << "and should not be used in models for which water use is an important output.");

    // create the definition
    openstudio::model::HotWaterEquipmentDefinition definition(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      definition.setName(*s + " Definition");
    }

    s = workspaceObject.getString(openstudio::HotWaterEquipmentFields::DesignLevelCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("EquipmentLevel", *s)) {
      d = workspaceObject.getDouble(openstudio::HotWaterEquipmentFields::DesignLevel);
      if (d) {
        definition.setDesignLevel(*d);
      } else {
        LOG(Error, "EquipmentLevel value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::HotWaterEquipmentFields::PowerperZoneFloorArea);
      if (d) {
        definition.setWattsperSpaceFloorArea(*d);
      } else {
        LOG(Error, "Watts/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Watts/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::HotWaterEquipmentFields::PowerperPerson);
      if (d) {
        definition.setWattsperPerson(*d);
      } else {
        LOG(Error, "Watts/Person value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::HotWaterEquipmentFields::FractionLatent);
    if (d) {
      definition.setFractionLatent(*d);
    }

    d = workspaceObject.getDouble(openstudio::HotWaterEquipmentFields::FractionRadiant);
    if (d) {
      definition.setFractionRadiant(*d);
    }

    d = workspaceObject.getDouble(openstudio::HotWaterEquipmentFields::FractionLost);
    if (d) {
      definition.setFractionLost(*d);
    }

    // create the instance
    HotWaterEquipment hotWaterEquipment(definition);

    s = workspaceObject.name();
    if (s) {
      hotWaterEquipment.setName(*s);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::HotWaterEquipmentFields::ZoneorZoneListorSpaceorSpaceListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          hotWaterEquipment.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          hotWaterEquipment.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::HotWaterEquipmentFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          hotWaterEquipment.setSchedule(schedule);
        }
      }
    }

    s = workspaceObject.getString(openstudio::HotWaterEquipmentFields::EndUseSubcategory);
    if (s) {
      hotWaterEquipment.setEndUseSubcategory(*s);
    }

    return hotWaterEquipment;
  }

}  // namespace energyplus

}  // namespace openstudio
