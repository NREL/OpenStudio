/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ExteriorFuelEquipment.hpp"
#include "../../model/ExteriorFuelEquipmentDefinition.hpp"

#include "../../model/Schedule.hpp"

#include <utilities/idd/Exterior_FuelEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExteriorFuelEquipment(model::ExteriorFuelEquipment& modelObject) {
    IdfObject idfObject(IddObjectType::Exterior_FuelEquipment);
    m_idfObjects.push_back(idfObject);
    idfObject.setString(Exterior_FuelEquipmentFields::Name, modelObject.name().get());

    auto sch = modelObject.schedule();
    OptionalIdfObject relatedIdfObject = translateAndMapModelObject(sch);
    OS_ASSERT(relatedIdfObject);
    idfObject.setString(Exterior_FuelEquipmentFields::ScheduleName, relatedIdfObject->name().get());

    // Get the Design Level from the attached equipment definition, and take multiplier into account
    model::ExteriorFuelEquipmentDefinition definition = modelObject.exteriorFuelEquipmentDefinition();
    double designLevel = definition.designLevel() * modelObject.multiplier();
    idfObject.setDouble(Exterior_FuelEquipmentFields::DesignLevel, designLevel);

    // Fuel Use Type
    idfObject.setString(Exterior_FuelEquipmentFields::FuelUseType, modelObject.fuelType());

    // End Use Subcategory
    idfObject.setString(Exterior_FuelEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
