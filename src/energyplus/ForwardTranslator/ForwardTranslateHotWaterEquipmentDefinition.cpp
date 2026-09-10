/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/HotWaterEquipmentDefinition.hpp"
#include "../../model/HotWaterEquipmentDefinition_Impl.hpp"

#include <utilities/idd/HotWaterEquipment_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHotWaterEquipmentDefinition(HotWaterEquipmentDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HotWaterEquipment_Definition, modelObject);

    idfObject.setString(HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod, modelObject.designLevelCalculationMethod());

    if (boost::optional<double> d = modelObject.designLevel()) {
      idfObject.setDouble(HotWaterEquipment_DefinitionFields::DesignLevel, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(HotWaterEquipment_DefinitionFields::PowerperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperPerson()) {
      idfObject.setDouble(HotWaterEquipment_DefinitionFields::PowerperPerson, *d);
    }

    if (!modelObject.isFractionLatentDefaulted()) {
      idfObject.setDouble(HotWaterEquipment_DefinitionFields::FractionLatent, modelObject.fractionLatent());
    }

    if (!modelObject.isFractionRadiantDefaulted()) {
      idfObject.setDouble(HotWaterEquipment_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());
    }

    if (!modelObject.isFractionLostDefaulted()) {
      idfObject.setDouble(HotWaterEquipment_DefinitionFields::FractionLost, modelObject.fractionLost());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
