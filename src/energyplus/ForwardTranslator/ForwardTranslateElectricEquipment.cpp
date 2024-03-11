/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricEquipment(ElectricEquipment& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricEquipment, modelObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    ElectricEquipmentDefinition definition = modelObject.electricEquipmentDefinition();

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.schedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(ElectricEquipmentFields::ScheduleName, idf_schedule_->nameString());
    }

    idfObject.setString(ElectricEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

    double multiplier = modelObject.multiplier();

    OptionalDouble d = definition.designLevel();
    if (d) {
      idfObject.setDouble(ElectricEquipmentFields::DesignLevel, (*d) * multiplier);
    }

    d = definition.wattsperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(ElectricEquipmentFields::WattsperFloorArea, (*d) * multiplier);
    }

    d = definition.wattsperPerson();
    if (d) {
      idfObject.setDouble(ElectricEquipmentFields::WattsperPerson, (*d) * multiplier);
    }

    if (!definition.isFractionLatentDefaulted()) {
      idfObject.setDouble(ElectricEquipmentFields::FractionLatent, definition.fractionLatent());
    }

    if (!definition.isFractionRadiantDefaulted()) {
      idfObject.setDouble(ElectricEquipmentFields::FractionRadiant, definition.fractionRadiant());
    }

    if (!definition.isFractionLostDefaulted()) {
      idfObject.setDouble(ElectricEquipmentFields::FractionLost, definition.fractionLost());
    }

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(ElectricEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
