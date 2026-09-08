/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SteamEquipmentDefinition.hpp"
#include "../../model/SteamEquipmentDefinition_Impl.hpp"

#include <utilities/idd/SteamEquipment_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSteamEquipmentDefinition(SteamEquipmentDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SteamEquipment_Definition, modelObject);

    idfObject.setString(SteamEquipment_DefinitionFields::DesignLevelCalculationMethod, modelObject.designLevelCalculationMethod());

    if (boost::optional<double> d = modelObject.designLevel()) {
      idfObject.setDouble(SteamEquipment_DefinitionFields::DesignLevel, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(SteamEquipment_DefinitionFields::PowerperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperPerson()) {
      idfObject.setDouble(SteamEquipment_DefinitionFields::PowerperPerson, *d);
    }

    if (!modelObject.isFractionLatentDefaulted()) {
      idfObject.setDouble(SteamEquipment_DefinitionFields::FractionLatent, modelObject.fractionLatent());
    }

    if (!modelObject.isFractionRadiantDefaulted()) {
      idfObject.setDouble(SteamEquipment_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());
    }

    if (!modelObject.isFractionLostDefaulted()) {
      idfObject.setDouble(SteamEquipment_DefinitionFields::FractionLost, modelObject.fractionLost());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
