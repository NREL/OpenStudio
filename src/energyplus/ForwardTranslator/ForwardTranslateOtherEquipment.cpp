/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/OtherEquipmentDefinition.hpp"
#include "../../model/OtherEquipmentDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/OtherEquipment_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOtherEquipment(OtherEquipment& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::OtherEquipment);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    OtherEquipmentDefinition definition = modelObject.otherEquipmentDefinition();

    idfObject.setString(OtherEquipmentFields::Name, modelObject.name().get());

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(OtherEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.schedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(OtherEquipmentFields::ScheduleName, idf_schedule_->nameString());
    }
    idfObject.setString(OtherEquipmentFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

    double multiplier = modelObject.multiplier();

    OptionalDouble d = definition.designLevel();
    if (d) {
      idfObject.setDouble(OtherEquipmentFields::DesignLevel, (*d) * multiplier);
    }

    d = definition.wattsperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(OtherEquipmentFields::PowerperFloorArea, (*d) * multiplier);
    }

    d = definition.wattsperPerson();
    if (d) {
      idfObject.setDouble(OtherEquipmentFields::PowerperPerson, (*d) * multiplier);
    }

    if (!definition.isFractionLatentDefaulted()) {
      idfObject.setDouble(OtherEquipmentFields::FractionLatent, definition.fractionLatent());
    }

    if (!definition.isFractionRadiantDefaulted()) {
      idfObject.setDouble(OtherEquipmentFields::FractionRadiant, definition.fractionRadiant());
    }

    if (!definition.isFractionLostDefaulted()) {
      idfObject.setDouble(OtherEquipmentFields::FractionLost, definition.fractionLost());
    }

    idfObject.setString(OtherEquipmentFields::FuelType, modelObject.fuelType());

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(OtherEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
