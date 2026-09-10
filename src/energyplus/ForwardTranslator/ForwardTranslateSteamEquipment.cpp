/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SteamEquipment.hpp"
#include "../../model/SteamEquipment_Impl.hpp"
#include "../../model/SteamEquipmentDefinition.hpp"
#include "../../model/SteamEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/SteamEquipment_FieldEnums.hxx>
#include <utilities/idd/SteamEquipment_Instance_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSteamEquipment(SteamEquipment& modelObject) {
    if (m_forwardTranslatorOptions.excludeSpaceLoadInstances()) {
      return translateSteamEquipmentLegacy(modelObject);
    }
    return translateSteamEquipmentInstance(modelObject);
  }

  boost::optional<IdfObject> ForwardTranslator::translateSteamEquipmentLegacy(SteamEquipment& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SteamEquipment);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    SteamEquipmentDefinition definition = modelObject.steamEquipmentDefinition();

    idfObject.setString(SteamEquipmentFields::Name, modelObject.name().get());

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(SteamEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.schedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(SteamEquipmentFields::ScheduleName, idf_schedule_->nameString());
    }

    idfObject.setString(SteamEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

    double multiplier = modelObject.multiplier();

    OptionalDouble d = definition.designLevel();
    if (d) {
      idfObject.setDouble(SteamEquipmentFields::DesignLevel, (*d) * multiplier);
    }

    d = definition.wattsperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(SteamEquipmentFields::PowerperFloorArea, (*d) * multiplier);
    }

    d = definition.wattsperPerson();
    if (d) {
      idfObject.setDouble(SteamEquipmentFields::PowerperPerson, (*d) * multiplier);
    }

    if (!definition.isFractionLatentDefaulted()) {
      idfObject.setDouble(SteamEquipmentFields::FractionLatent, definition.fractionLatent());
    }

    if (!definition.isFractionRadiantDefaulted()) {
      idfObject.setDouble(SteamEquipmentFields::FractionRadiant, definition.fractionRadiant());
    }

    if (!definition.isFractionLostDefaulted()) {
      idfObject.setDouble(SteamEquipmentFields::FractionLost, definition.fractionLost());
    }

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(SteamEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateSteamEquipmentInstance(SteamEquipment& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SteamEquipment_Instance, modelObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    SteamEquipmentDefinition definition = modelObject.steamEquipmentDefinition();
    auto definitionIdfObject_ = translateAndMapModelObject(definition);
    OS_ASSERT(definitionIdfObject_);
    idfObject.setString(SteamEquipment_InstanceFields::SteamEquipmentDefinitionName, definitionIdfObject_->nameString());

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(SteamEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.schedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(SteamEquipment_InstanceFields::ScheduleName, idf_schedule_->nameString());
    }

    idfObject.setDouble(SteamEquipment_InstanceFields::Multiplier, modelObject.multiplier());

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(SteamEquipment_InstanceFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
