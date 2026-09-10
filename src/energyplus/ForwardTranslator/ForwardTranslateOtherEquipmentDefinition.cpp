/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/OtherEquipmentDefinition.hpp"
#include "../../model/OtherEquipmentDefinition_Impl.hpp"

#include <utilities/idd/OtherEquipment_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOtherEquipmentDefinition(OtherEquipmentDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::OtherEquipment_Definition, modelObject);

    idfObject.setString(OtherEquipment_DefinitionFields::DesignLevelCalculationMethod, modelObject.designLevelCalculationMethod());

    if (boost::optional<double> d = modelObject.designLevel()) {
      idfObject.setDouble(OtherEquipment_DefinitionFields::DesignLevel, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(OtherEquipment_DefinitionFields::PowerperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperPerson()) {
      idfObject.setDouble(OtherEquipment_DefinitionFields::PowerperPerson, *d);
    }

    if (!modelObject.isFractionLatentDefaulted()) {
      idfObject.setDouble(OtherEquipment_DefinitionFields::FractionLatent, modelObject.fractionLatent());
    }

    if (!modelObject.isFractionRadiantDefaulted()) {
      idfObject.setDouble(OtherEquipment_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());
    }

    if (!modelObject.isFractionLostDefaulted()) {
      idfObject.setDouble(OtherEquipment_DefinitionFields::FractionLost, modelObject.fractionLost());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
