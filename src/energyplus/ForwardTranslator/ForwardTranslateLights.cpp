/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Lights.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/Space.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/LifeCycleCost.hpp"
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateLights(Lights& modelObject) {
    // create, register, and name object
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Lights, modelObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    LightsDefinition definition = modelObject.lightsDefinition();

    IdfObject parentIdfObject = getSpaceLoadParent(modelObject);
    idfObject.setString(LightsFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    if (boost::optional<Schedule> schedule = modelObject.schedule()) {
      auto idf_schedule_ = translateAndMapModelObject(*schedule);
      OS_ASSERT(idf_schedule_);
      idfObject.setString(LightsFields::ScheduleName, idf_schedule_->nameString());
    }

    idfObject.setString(LightsFields::DesignLevelCalculationMethod, definition.designLevelCalculationMethod());

    double multiplier = modelObject.multiplier();

    boost::optional<double> d = definition.lightingLevel();
    if (d) {
      idfObject.setDouble(LightsFields::LightingLevel, (*d) * multiplier);
    }

    d = definition.wattsperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(LightsFields::WattsperZoneFloorArea, (*d) * multiplier);
    }

    d = definition.wattsperPerson();
    if (d) {
      idfObject.setDouble(LightsFields::WattsperPerson, (*d) * multiplier);
    }

    if (!definition.isReturnAirFractionDefaulted()) {
      idfObject.setDouble(LightsFields::ReturnAirFraction, definition.returnAirFraction());
    }

    if (!definition.isFractionRadiantDefaulted()) {
      idfObject.setDouble(LightsFields::FractionRadiant, definition.fractionRadiant());
    }

    if (!definition.isFractionVisibleDefaulted()) {
      idfObject.setDouble(LightsFields::FractionVisible, definition.fractionVisible());
    }

    if (!modelObject.isFractionReplaceableDefaulted()) {
      idfObject.setDouble(LightsFields::FractionReplaceable, modelObject.fractionReplaceable());
    }

    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(LightsFields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    if (!definition.isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted()) {
      if (definition.returnAirFractionCalculatedfromPlenumTemperature()) {
        idfObject.setString(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature, "Yes");
      } else {
        idfObject.setString(LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature, "No");
      }
    }

    if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted()) {
      idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,
                          definition.returnAirFractionFunctionofPlenumTemperatureCoefficient1());
    }

    if (!definition.isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted()) {
      idfObject.setDouble(LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,
                          definition.returnAirFractionFunctionofPlenumTemperatureCoefficient2());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
