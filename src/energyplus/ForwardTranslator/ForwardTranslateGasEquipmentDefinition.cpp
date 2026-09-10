/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"

#include <utilities/idd/GasEquipment_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGasEquipmentDefinition(GasEquipmentDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::GasEquipment_Definition, modelObject);

    idfObject.setString(GasEquipment_DefinitionFields::DesignLevelCalculationMethod, modelObject.designLevelCalculationMethod());

    if (boost::optional<double> d = modelObject.designLevel()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::DesignLevel, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::PowerperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperPerson()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::PowerperPerson, *d);
    }

    if (!modelObject.isFractionLatentDefaulted()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::FractionLatent, modelObject.fractionLatent());
    }

    if (!modelObject.isFractionRadiantDefaulted()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());
    }

    if (!modelObject.isFractionLostDefaulted()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::FractionLost, modelObject.fractionLost());
    }

    if (!modelObject.isCarbonDioxideGenerationRateDefaulted()) {
      idfObject.setDouble(GasEquipment_DefinitionFields::CarbonDioxideGenerationRate, modelObject.carbonDioxideGenerationRate());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
