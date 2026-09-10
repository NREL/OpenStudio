/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"

#include <utilities/idd/ElectricEquipment_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricEquipmentDefinition(ElectricEquipmentDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricEquipment_Definition, modelObject);

    idfObject.setString(ElectricEquipment_DefinitionFields::DesignLevelCalculationMethod, modelObject.designLevelCalculationMethod());

    if (boost::optional<double> d = modelObject.designLevel()) {
      idfObject.setDouble(ElectricEquipment_DefinitionFields::DesignLevel, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(ElectricEquipment_DefinitionFields::WattsperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperPerson()) {
      idfObject.setDouble(ElectricEquipment_DefinitionFields::WattsperPerson, *d);
    }

    if (!modelObject.isFractionLatentDefaulted()) {
      idfObject.setDouble(ElectricEquipment_DefinitionFields::FractionLatent, modelObject.fractionLatent());
    }

    if (!modelObject.isFractionRadiantDefaulted()) {
      idfObject.setDouble(ElectricEquipment_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());
    }

    if (!modelObject.isFractionLostDefaulted()) {
      idfObject.setDouble(ElectricEquipment_DefinitionFields::FractionLost, modelObject.fractionLost());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
