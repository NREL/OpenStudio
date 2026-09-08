/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"

#include <utilities/idd/Lights_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateLightsDefinition(LightsDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Lights_Definition, modelObject);

    idfObject.setString(Lights_DefinitionFields::DesignLevelCalculationMethod, modelObject.designLevelCalculationMethod());

    if (boost::optional<double> d = modelObject.lightingLevel()) {
      idfObject.setDouble(Lights_DefinitionFields::LightingLevel, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(Lights_DefinitionFields::WattsperFloorArea, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperPerson()) {
      idfObject.setDouble(Lights_DefinitionFields::WattsperPerson, *d);
    }

    if (!modelObject.isReturnAirFractionDefaulted()) {
      idfObject.setDouble(Lights_DefinitionFields::ReturnAirFraction, modelObject.returnAirFraction());
    }

    if (!modelObject.isFractionRadiantDefaulted()) {
      idfObject.setDouble(Lights_DefinitionFields::FractionRadiant, modelObject.fractionRadiant());
    }

    if (!modelObject.isFractionVisibleDefaulted()) {
      idfObject.setDouble(Lights_DefinitionFields::FractionVisible, modelObject.fractionVisible());
    }

    if (!modelObject.isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted()) {
      if (modelObject.returnAirFractionCalculatedfromPlenumTemperature()) {
        idfObject.setString(Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "Yes");
      } else {
        idfObject.setString(Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "No");
      }
    }

    if (!modelObject.isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted()) {
      idfObject.setDouble(Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,
                           modelObject.returnAirFractionFunctionofPlenumTemperatureCoefficient1());
    }

    if (!modelObject.isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted()) {
      idfObject.setDouble(Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,
                           modelObject.returnAirFractionFunctionofPlenumTemperatureCoefficient2());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
