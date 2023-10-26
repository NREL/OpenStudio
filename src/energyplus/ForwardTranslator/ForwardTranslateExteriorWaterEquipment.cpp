/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ExteriorWaterEquipment.hpp"
#include "../../model/ExteriorWaterEquipmentDefinition.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Exterior_WaterEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExteriorWaterEquipment(model::ExteriorWaterEquipment& modelObject) {
    IdfObject idfObject(IddObjectType::Exterior_WaterEquipment);
    m_idfObjects.push_back(idfObject);
    idfObject.setString(Exterior_WaterEquipmentFields::Name, modelObject.name().get());

    auto sch = modelObject.schedule();
    OptionalIdfObject relatedIdfObject = translateAndMapModelObject(sch);
    OS_ASSERT(relatedIdfObject);
    idfObject.setString(Exterior_WaterEquipmentFields::ScheduleName, relatedIdfObject->name().get());

    // Get the Design Level from the attached equipment definition, and take multiplier into account
    model::ExteriorWaterEquipmentDefinition definition = modelObject.exteriorWaterEquipmentDefinition();
    double designLevel = definition.designLevel() * modelObject.multiplier();
    idfObject.setDouble(Exterior_WaterEquipmentFields::DesignLevel, designLevel);

    // Fuel Use Type: always water since it's the only valid choice
    idfObject.setString(Exterior_WaterEquipmentFields::FuelUseType, "Water");

    // End Use Subcategory
    idfObject.setString(Exterior_WaterEquipmentFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
